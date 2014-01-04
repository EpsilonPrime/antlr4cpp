﻿#include "UnbufferedTokenStream.h"
#include "TokenSource.h"

/*
 * [The "BSD license"]
 *  Copyright (c) 2013 Terence Parr
 *  Copyright (c) 2013 Dan McLaughlin
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 *  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 *  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 *  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 *  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                
                template<typename T>                
                UnbufferedTokenStream::UnbufferedTokenStream(TokenSource *tokenSource) { //this(tokenSource, 256);
                }
                
                UnbufferedTokenStream::UnbufferedTokenStream(TokenSource *tokenSource, int bufferSize) {
                    InitializeInstanceFields();
                    this->tokenSource = tokenSource;
                    Token = new Token[bufferSize];
                    n = 0;
                    fill(1); // prime the pump
                }
                
                virtual Token *UnbufferedTokenStream::get(int i) override { // get absolute index
                    int bufferStartIndex = getBufferStartIndex();
                    if (i < bufferStartIndex || i >= bufferStartIndex + n) {
                        throw IndexOutOfBoundsException(std::wstring(L"get(") + i + std::wstring(L") outside buffer: ") + bufferStartIndex + std::wstring(L"..") + (bufferStartIndex + n));
                    }
                    return tokens[i - bufferStartIndex];
                }
                
                virtual Token *UnbufferedTokenStream::LT(int i) override {
                    if (i == -1) {
                        return lastToken;
                    }
                    
                    sync(i);
                    int index = p + i - 1;
                    if (index < 0) {
                        throw IndexOutOfBoundsException(std::wstring(L"LT(") + i + std::wstring(L") gives negative index"));
                    }
                    
                    if (index >= n) {
                        assert(n > 0 && tokens[n - 1]->getType() == Token::EOF);
                        return tokens[n - 1];
                    }
                    
                    return tokens[index];
                }
                
                virtual int UnbufferedTokenStream::LA(int i) override {
                    return LT(i)->getType();
                }
                
                virtual TokenSource *UnbufferedTokenStream::getTokenSource() override {
                    return tokenSource;
                }
                
                virtual std::wstring UnbufferedTokenStream::getText() override {
                    return L"";
                }
                
                virtual std::wstring UnbufferedTokenStream::getText(RuleContext *ctx) override {
                    return getText(ctx->getSourceInterval());
                }
                
                virtual std::wstring UnbufferedTokenStream::getText(Token *start, Token *stop) override {
                    return getText(Interval::of(start->getTokenIndex(), stop->getTokenIndex()));
                }
                
                virtual void UnbufferedTokenStream::consume() override {
                    if (LA(1) == Token::EOF) {
                        throw IllegalStateException(L"cannot consume EOF");
                    }
                    
                    // buf always has at least tokens[p==0] in this method due to ctor
                    lastToken = tokens[p]; // track last token for LT(-1)
                    
                    // if we're at last token and no markers, opportunity to flush buffer
                    if (p == n - 1 && numMarkers == 0) {
                        n = 0;
                        p = -1; // p++ will leave this at 0
                        lastTokenBufferStart = lastToken;
                    }
                    
                    p++;
                    currentTokenIndex++;
                    sync(1);
                }
                
                /// <summary>
                /// Make sure we have 'need' elements from current position <seealso cref="#p p"/>. Last valid
                ///  {@code p} index is {@code tokens.length-1}.  {@code p+need-1} is the tokens index 'need' elements
                ///  ahead.  If we need 1 element, {@code (p+1-1)==p} must be less than {@code tokens.length}.
                /// </summary>
                virtual void UnbufferedTokenStream::sync(int want) {
                    int need = (p + want - 1) - n + 1; // how many more elements we need?
                    if (need > 0) {
                        fill(need);
                    }
                }
                
                /// <summary>
                /// Add {@code n} elements to the buffer. Returns the number of tokens
                /// actually added to the buffer. If the return value is less than {@code n},
                /// then EOF was reached before {@code n} tokens could be added.
                /// </summary>
                virtual int UnbufferedTokenStream::fill(int n) {
                    for (int i = 0; i < n; i++) {
                        if (this->n > 0 && tokens[this->n - 1]->getType() == Token::EOF) {
                            return i;
                        }
                        
                        Token *t = tokenSource->nextToken();
                        add(t);
                    }
                    
                    return n;
                }
                
                virtual void UnbufferedTokenStream::add(Token *t) {
                    if (n >= tokens->length) {
                        tokens = Arrays::copyOf(tokens, tokens->length * 2);
                    }
                    
                    if (dynamic_cast<WritableToken*>(t) != nullptr) {
                        (static_cast<WritableToken*>(t))->setTokenIndex(getBufferStartIndex() + n);
                    }
                    
                    tokens[n++] = t;
                }
                
                /// <summary>
                /// Return a marker that we can release later.
                /// <p/>
                /// The specific marker value used for this class allows for some level of
                /// protection against misuse where {@code seek()} is called on a mark or
                /// {@code release()} is called in the wrong order.
                /// </summary>
                virtual int mark() UnbufferedTokenStream::override {
                    if (numMarkers == 0) {
                        lastTokenBufferStart = lastToken;
                    }
                    
                    int mark = -numMarkers - 1;
                    numMarkers++;
                    return mark;
                }
                
                virtual void UnbufferedTokenStream::release(int marker) override {
                    int expectedMark = -numMarkers;
                    if (marker != expectedMark) {
                        throw IllegalStateException(L"release() called with an invalid marker.");
                    }
                    
                    numMarkers--;
                    if (numMarkers == 0) { // can we release buffer?
                        if (p > 0) {
                            // Copy tokens[p]..tokens[n-1] to tokens[0]..tokens[(n-1)-p], reset ptrs
                            // p is last valid token; move nothing if p==n as we have no valid char
                            System::arraycopy(tokens, p, tokens, 0, n - p); // shift n-p tokens from p to 0
                            n = n - p;
                            p = 0;
                        }
                        
                        lastTokenBufferStart = lastToken;
                    }
                }
                
                virtual int UnbufferedTokenStream::index() override {
                    return currentTokenIndex;
                }
                
                virtual void UnbufferedTokenStream::seek(int index) override { // seek to absolute index
                    if (index == currentTokenIndex) {
                        return;
                    }
                    
                    if (index > currentTokenIndex) {
                        sync(index - currentTokenIndex);
                        index = std::min(index, getBufferStartIndex() + n - 1);
                    }
                    
                    int bufferStartIndex = getBufferStartIndex();
                    int i = index - bufferStartIndex;
                    if (i < 0) {
                        throw IllegalArgumentException(std::wstring(L"cannot seek to negative index ") + index);
                    } else if (i >= n) {
                        throw UnsupportedOperationException(std::wstring(L"seek to index outside buffer: ") + index + std::wstring(L" not in ") + bufferStartIndex + std::wstring(L"..") + (bufferStartIndex + n));
                    }
                    
                    p = i;
                    currentTokenIndex = index;
                    if (p == 0) {
                        lastToken = lastTokenBufferStart;
                    } else {
                        lastToken = tokens[p - 1];
                    }
                }
                
                virtual int UnbufferedTokenStream::size() override {
                    throw UnsupportedOperationException(L"Unbuffered stream cannot know its size");
                }
                
                virtual std::wstring UnbufferedTokenStream::getSourceName() override {
                    return tokenSource->getSourceName();
                }
                
                virtual std::wstring UnbufferedTokenStream::getText(Interval *interval) override {
                    int bufferStartIndex = getBufferStartIndex();
                    int bufferStopIndex = bufferStartIndex + tokens->length - 1;
                    
                    int start = interval->a;
                    int stop = interval->b;
                    if (start < bufferStartIndex || stop > bufferStopIndex) {
                        throw UnsupportedOperationException(std::wstring(L"interval ") + interval + std::wstring(L" not in token buffer window: ") + bufferStartIndex + std::wstring(L"..") + bufferStopIndex);
                    }
                    
                    int a = start - bufferStartIndex;
                    int b = stop - bufferStartIndex;
                    
                    StringBuilder *buf = new StringBuilder();
                    for (int i = a; i <= b; i++) {
                        Token *t = tokens[i];
                        buf->append(t->getText());
                    }
                    
                    //JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                    return buf->toString();
                }
                
                int UnbufferedTokenStream::getBufferStartIndex() {
                    return currentTokenIndex - p;
                }
                
                void UnbufferedTokenStream::InitializeInstanceFields() {
                    n = 0;
                    p = 0;
                    numMarkers = 0;
                    currentTokenIndex = 0;
                }
                
                
                
            }
        }
    }
}
