﻿#pragma once

#include "ParseTree.h"
#include "ParseTreeListener.h"
#include "RuleNode.h"

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
                namespace tree {

                    using org::antlr::v4::runtime::ParserRuleContext;
                    using org::antlr::v4::runtime::RuleContext;

                    class ParseTreeWalker {
                    public:
                        static ParseTreeWalker *const DEFAULT;

                        virtual void walk(ParseTreeListener *listener, ParseTree *t);

                        /// <summary>
                        /// The discovery of a rule node, involves sending two events: the generic
                        /// <seealso cref="ParseTreeListener#enterEveryRule"/> and a
                        /// <seealso cref="RuleContext"/>-specific event. First we trigger the generic and then
                        /// the rule specific. We to them in reverse order upon finishing the node.
                        /// </summary>
                    protected:
                        virtual void enterRule(ParseTreeListener *listener, RuleNode *r);

                        virtual void exitRule(ParseTreeListener *listener, RuleNode *r);
                    };

                }
            }
        }
    }
}
