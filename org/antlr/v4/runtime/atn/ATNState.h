﻿#pragma once

#include "Declarations.h"
#include "stringconverter.h"

#include <string>
#include <vector>
#include <list>



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
                namespace atn {


                    /// <summary>
                    /// The following images show the relation of states and
                    /// <seealso cref="ATNState#transitions"/> for various grammar constructs.
                    /// 
                    /// <ul>
                    /// 
                    /// <li>Solid edges marked with an &#0949; indicate a required
                    /// <seealso cref="EpsilonTransition"/>.</li>
                    /// 
                    /// <li>Dashed edges indicate locations where any transition derived from
                    /// <seealso cref="Transition"/> might appear.</li>
                    /// 
                    /// <li>Dashed nodes are place holders for either a sequence of linked
                    /// <seealso cref="BasicState"/> states or the inclusion of a block representing a nested
                    /// construct in one of the forms below.</li>
                    /// 
                    /// <li>Nodes showing multiple outgoing alternatives with a {@code ...} support
                    /// any number of alternatives (one or more). Nodes without the {@code ...} only
                    /// support the exact number of alternatives shown in the diagram.</li>
                    /// 
                    /// </ul>
                    /// 
                    /// <h2>Basic Blocks</h2>
                    /// 
                    /// <h3>Rule</h3>
                    /// 
                    /// <embed src="images/Rule.svg" type="image/svg+xml"/>
                    /// 
                    /// <h3>Block of 1 or more alternatives</h3>
                    /// 
                    /// <embed src="images/Block.svg" type="image/svg+xml"/>
                    /// 
                    /// <h2>Greedy Loops</h2>
                    /// 
                    /// <h3>Greedy Closure: {@code (...)*}</h3>
                    /// 
                    /// <embed src="images/ClosureGreedy.svg" type="image/svg+xml"/>
                    /// 
                    /// <h3>Greedy Positive Closure: {@code (...)+}</h3>
                    /// 
                    /// <embed src="images/PositiveClosureGreedy.svg" type="image/svg+xml"/>
                    /// 
                    /// <h3>Greedy Optional: {@code (...)?}</h3>
                    /// 
                    /// <embed src="images/OptionalGreedy.svg" type="image/svg+xml"/>
                    /// 
                    /// <h2>Non-Greedy Loops</h2>
                    /// 
                    /// <h3>Non-Greedy Closure: {@code (...)*?}</h3>
                    /// 
                    /// <embed src="images/ClosureNonGreedy.svg" type="image/svg+xml"/>
                    /// 
                    /// <h3>Non-Greedy Positive Closure: {@code (...)+?}</h3>
                    /// 
                    /// <embed src="images/PositiveClosureNonGreedy.svg" type="image/svg+xml"/>
                    /// 
                    /// <h3>Non-Greedy Optional: {@code (...)??}</h3>
                    /// 
                    /// <embed src="images/OptionalNonGreedy.svg" type="image/svg+xml"/>
                    /// </summary>
                    class ATNState {
                    public:
                        static const int INITIAL_NUM_TRANSITIONS = 4;

                        // constants for serialization
                        static const int INVALID_TYPE = 0;
                        static const int BASIC = 1;
                        static const int RULE_START = 2;
                        static const int BLOCK_START = 3;
                        static const int PLUS_BLOCK_START = 4;
                        static const int STAR_BLOCK_START = 5;
                        static const int TOKEN_START = 6;
                        static const int RULE_STOP = 7;
                        static const int BLOCK_END = 8;
                        static const int STAR_LOOP_BACK = 9;
                        static const int STAR_LOOP_ENTRY = 10;
                        static const int PLUS_LOOP_BACK = 11;
                        static const int LOOP_END = 12;

                        static const std::vector<std::wstring> serializationNames;

                        static const int INVALID_STATE_NUMBER = -1;

                        /// <summary>
                        /// Which ATN are we in? </summary>
                        ATN *atn = nullptr;

                        int stateNumber;

                        int ruleIndex; // at runtime, we don't have Rule objects

                        bool epsilonOnlyTransitions;

                        /// <summary>
                        /// Track the transitions emanating from this ATN state. </summary>
                    // TODO protected:???  It's being accessed directly in ATNDeserializer
                        std::vector<Transition*> transitions;

                        /// <summary>
                        /// Used to cache lookahead during parsing, not used during construction </summary>
                    public:
                        misc::IntervalSet *nextTokenWithinRule;

                        virtual int hashCode();
                        virtual bool equals(void *o);

                        virtual bool isNonGreedyExitState();

                        virtual std::wstring toString();

                        virtual Transition *getTransitions();

                        virtual int getNumberOfTransitions();

                        virtual void addTransition(Transition *e);

                        virtual void addTransition(int index, Transition *e);

                        virtual Transition *transition(int i);

                        virtual void setTransition(int i, Transition *e);

                        virtual Transition *removeTransition(int index);

                        virtual int getStateType() = 0;

                        bool onlyHasEpsilonTransitions();

                        virtual void setRuleIndex(int ruleIndex);
                        
                        virtual ~ATNState() = 0;

                    private:
                        void InitializeInstanceFields();

public:
//                        ATNState() : transitions(new std::list<Transition*>()) {
//                            InitializeInstanceFields();
//                        }
                    };

                }
            }
        }
    }
}
