///
/// @file codecvt_utf8.hpp
/// @brief code conversation fom utf8 to unicode and reverse
///
/// Detailed description starts here.
///
/// @author Michael Koch (mk), uockc@stud.uni-karlsruhe.de
///
/// @internal
/// Created 12/16/2010
/// Revision ---
/// Compiler gcc/g++
/// Company Universität Karlsruhe, Germany
/// Copyright Copyright (c) 2010, Michael Koch
///
/// This source code is released for free distribution under the terms of the
/// GNU General Public License as published by the Free Software Foundation.
///=====================================================================================
///

#ifndef _CODE_CODECVT_UTF8_HPP_
#define _CODE_CODECVT_UTF8_HPP_

#include <cassert>
#include <locale>

namespace code {

        
        ///
        /// @class codecvt_utf8_state
        /// @brief state of utf8 encoding procedure
        ///
        ///
        template <class internT>
        class codecvt_utf8_state {
                
        public:

                typedef internT intern_type;

                ///
                /// @brief constructor
                ///
                codecvt_utf8_state() {
                        left_encoding = 0;
                        left_decoding = 0;
                }


                ///
                /// @brief destructor
                ///
                virtual ~codecvt_utf8_state() {}

                codecvt_utf8_state& operator = (const codecvt_utf8_state& s)
                {
                        left_encoding = s.left_encoding;
                        left_decoding = s.left_decoding;
                        uc = s.uc;
                        return *this;
                }

                codecvt_utf8_state(const codecvt_utf8_state& s)
                {
                        *this = s;
                }

                ///
                /// @brief get stored unicode character
                ///
                intern_type unicode() const {return uc;}

                int left_characters_to_encode() const {return left_encoding;}
                int left_characters_to_decode() const {return left_decoding;}
                bool is_encoding_complete() const {return left_encoding == 0;}
                bool is_decoding_complete() const {return left_decoding == 0;}

                void encoded() {assert(left_encoding > 0); left_encoding--;}
                void decoded() {assert(left_decoding > 0); left_decoding--;}

                void decode(const int left) {assert(left > 0); left_decoding = left - 1;}
                void encode(const int left, const intern_type& uc) {assert(left>0); left_encoding=left-1;this->uc = uc;}

                bool is_valid() const {return true;}
        
        private:

                int left_encoding;
                int left_decoding;
                intern_type uc;

        };

        
        ///
        /// @class codecvt_utf8
        /// @brief support utf8 encoding and decoding in standard library streaming
        ///
        ///
        template <class externT, class internT>
        class codecvt_utf8
                : public std::codecvt<internT, externT, codecvt_utf8_state<internT>> {
        
        public:

                                
                        /// type of extern representation type
                        typedef typename std::codecvt<internT, externT, codecvt_utf8_state<internT>>::extern_type extern_type;
                        
                        
                        /// type of intern representation type
                        typedef typename std::codecvt<internT, externT, codecvt_utf8_state<internT>>::intern_type intern_type;
                        
                        
                        /// type of state representation type
                        typedef typename std::codecvt<internT, externT, codecvt_utf8_state<internT>>::state_type state_type;
                        
                        
                        static_assert(sizeof(intern_type) >= 4, "intern representation must be at least 4 bytes");

                        
                        ///
                        /// @brief constructor
                        /// @param strictness determine wether decoding procedure should be strictly or not
                        ///
                        codecvt_utf8 (const bool strictness = true);
                        
                        
                        ///
                        /// @brief destructor
                        ///
                        virtual ~codecvt_utf8 ();
                        
                        
                        ///
                        /// @brief assign object
                        /// @param ccvt utf8 code conversation object
                        /// @return reference on this object
                        ///
                        codecvt_utf8&
                        operator = (const codecvt_utf8& ccvt);

        
        protected:
                        
                        
                        ///
                        /// @copydoc
                        ///
                        virtual std::codecvt_base::result
                        do_out (state_type& state, const intern_type* from, const intern_type* from_end, const intern_type*& from_next,
                                                        extern_type* to, extern_type* to_limit, extern_type*& to_next)
                        const;


                        ///
                        /// @copydoc
                        ///
                        virtual std::codecvt_base::result
                        do_in (state_type& state, const extern_type* from, const extern_type* from_end, const extern_type*& from_next,
                                                intern_type* to, intern_type* to_limit, intern_type*& to_next)
                        const;


                        ///
                        /// @copydoc
                        ///
                        virtual std::codecvt_base::result
                        do_unshift (state_type& state, extern_type* to, extern_type* to_limit, extern_type*& to_next)
                        const;


                        ///
                        /// @copydoc
                        ///
                        virtual int
                        do_encoding ()
                        const throw();


                        ///
                        /// @copydoc
                        ///
                        virtual bool
                        do_always_noconv ()
                        const throw();


                        ///
                        /// @copydoc
                        ///
                        virtual int
                        do_length (state_type& state, const extern_type* from, const extern_type* end, size_t max)
                        const;


                        ///
                        /// @copydoc
                        ///
                        virtual int
                        do_max_length ()
                        const throw();


        private:

        
                        /// determine wether decoding is going to be in a strict way
                        bool strictness;
        };

}

#include <codecvt_utf8.tcc>
#endif
