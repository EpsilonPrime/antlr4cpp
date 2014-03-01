﻿#pragma once

#include "Recognizer.h"
#include "Declarations.h"
#include <bitset>

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
                /// <summary>
                /// How to emit recognition errors. </summary>
                class ANTLRErrorListener {
#ifdef TODO
                    work on this number
#endif
                    static const int BITSET_SIZE = 1024;
                    
                    /// <summary>
                    /// Upon syntax error, notify any interested parties. This is not how to
                    /// recover from errors or compute error messages. <seealso cref="ANTLRErrorStrategy"/>
                    /// specifies how to recover from syntax errors and how to compute error
                    /// messages. This listener's job is simply to emit a computed message,
                    /// though it has enough information to create its own message in many cases.
                    /// <p/>
                    /// The <seealso cref="RecognitionException"/> is non-null for all syntax errors except
                    /// when we discover mismatched token errors that we can recover from
                    /// in-line, without returning from the surrounding rule (via the single
                    /// token insertion and deletion mechanism).
                    /// </summary>
                    /// <param name="recognizer">
                    ///        What parser got the error. From this
                    /// 		  object, you can access the context as well
                    /// 		  as the input stream. </param>
                    /// <param name="offendingSymbol">
                    ///        The offending token in the input token
                    /// 		  stream, unless recognizer is a lexer (then it's null). If
                    /// 		  no viable alternative error, {@code e} has token at which we
                    /// 		  started production for the decision. </param>
                    /// <param name="line">
                    /// 		  The line number in the input where the error occurred. </param>
                    /// <param name="charPositionInLine">
                    /// 		  The character position within that line where the error occurred. </param>
                    /// <param name="msg">
                    /// 		  The message to emit. </param>
                    /// <param name="e">
                    ///        The exception generated by the parser that led to
                    ///        the reporting of an error. It is null in the case where
                    ///        the parser was able to recover in line without exiting the
                    ///        surrounding rule. </param>
                public:

                    template<typename T1, typename T2>
                    void syntaxError(Recognizer<T1, T2> *recognizer, void *offendingSymbol, int line, int charPositionInLine, const std::wstring &msg, RecognitionException *e);

                    /// <summary>
                    /// This method is called by the parser when a full-context prediction
                    /// results in an ambiguity.
                    /// <p/>
                    /// When {@code exact} is {@code true}, <em>all</em> of the alternatives in
                    /// {@code ambigAlts} are viable, i.e. this is reporting an exact ambiguity.
                    /// When {@code exact} is {@code false}, <em>at least two</em> of the
                    /// alternatives in {@code ambigAlts} are viable for the current input, but
                    /// the prediction algorithm terminated as soon as it determined that at
                    /// least the <em>minimum</em> alternative in {@code ambigAlts} is viable.
                    /// <p/>
                    /// When the <seealso cref="PredictionMode#LL_EXACT_AMBIG_DETECTION"/> prediction mode
                    /// is used, the parser is required to identify exact ambiguities so
                    /// {@code exact} will always be {@code true}.
                    /// <p/>
                    /// This method is not used by lexers.
                    /// </summary>
                    /// <param name="recognizer"> the parser instance </param>
                    /// <param name="dfa"> the DFA for the current decision </param>
                    /// <param name="startIndex"> the input index where the decision started </param>
                    /// <param name="stopIndex"> the input input where the ambiguity is reported </param>
                    /// <param name="exact"> {@code true} if the ambiguity is exactly known, otherwise
                    /// {@code false}. This is always {@code true} when
                    /// <seealso cref="PredictionMode#LL_EXACT_AMBIG_DETECTION"/> is used. </param>
                    /// <param name="ambigAlts"> the potentially ambiguous alternatives </param>
                    /// <param name="configs"> the ATN configuration set where the ambiguity was
                    /// determined </param>
                    virtual void reportAmbiguity(Parser *recognizer, dfa::DFA *dfa, int startIndex, int stopIndex, bool exact, std::bitset<BITSET_SIZE> *ambigAlts, atn::ATNConfigSet *configs) = 0;

                    /// <summary>
                    /// This method is called when an SLL conflict occurs and the parser is about
                    /// to use the full context information to make an LL decision.
                    /// <p/>
                    /// If one or more configurations in {@code configs} contains a semantic
                    /// predicate, the predicates are evaluated before this method is called. The
                    /// subset of alternatives which are still viable after predicates are
                    /// evaluated is reported in {@code conflictingAlts}.
                    /// <p/>
                    /// This method is not used by lexers.
                    /// </summary>
                    /// <param name="recognizer"> the parser instance </param>
                    /// <param name="dfa"> the DFA for the current decision </param>
                    /// <param name="startIndex"> the input index where the decision started </param>
                    /// <param name="stopIndex"> the input index where the SLL conflict occurred </param>
                    /// <param name="conflictingAlts"> The specific conflicting alternatives. If this is
                    /// {@code null}, the conflicting alternatives are all alternatives
                    /// represented in {@code configs}. </param>
                    /// <param name="configs"> the ATN configuration set where the SLL conflict was
                    /// detected </param>
                    virtual void reportAttemptingFullContext(Parser *recognizer, dfa::DFA *dfa, int startIndex, int stopIndex, std::bitset<BITSET_SIZE> *conflictingAlts, atn::ATNConfigSet *configs) = 0;

                    /// <summary>
                    /// This method is called by the parser when a full-context prediction has a
                    /// unique result.
                    /// <p/>
                    /// For prediction implementations that only evaluate full-context
                    /// predictions when an SLL conflict is found (including the default
                    /// <seealso cref="ParserATNSimulator"/> implementation), this method reports cases
                    /// where SLL conflicts were resolved to unique full-context predictions,
                    /// i.e. the decision was context-sensitive. This report does not necessarily
                    /// indicate a problem, and it may appear even in completely unambiguous
                    /// grammars.
                    /// <p/>
                    /// {@code configs} may have more than one represented alternative if the
                    /// full-context prediction algorithm does not evaluate predicates before
                    /// beginning the full-context prediction. In all cases, the final prediction
                    /// is passed as the {@code prediction} argument.
                    /// <p/>
                    /// This method is not used by lexers.
                    /// </summary>
                    /// <param name="recognizer"> the parser instance </param>
                    /// <param name="dfa"> the DFA for the current decision </param>
                    /// <param name="startIndex"> the input index where the decision started </param>
                    /// <param name="stopIndex"> the input index where the context sensitivity was
                    /// finally determined </param>
                    /// <param name="prediction"> the unambiguous result of the full-context prediction </param>
                    /// <param name="configs"> the ATN configuration set where the unambiguous prediction
                    /// was determined </param>
                    virtual void reportContextSensitivity(Parser *recognizer, dfa::DFA *dfa, int startIndex, int stopIndex, int prediction, atn::ATNConfigSet *configs) = 0;
                };

            }
        }
    }
}
