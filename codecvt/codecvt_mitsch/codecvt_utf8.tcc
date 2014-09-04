///
/// @file codecvt_utf8.tcc
/// @brief code implementation of utf8 code conversation
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

#ifndef _CODE_CODECVT_UTF8_TCC_
#define _CODE_CODECVT_UTF8_TCC_

#include <locale>


namespace code {


                template <class externT, class internT>
        codecvt_utf8<externT, internT>::codecvt_utf8
                (const bool strictness)
        {
                        this->strictness = strictness;
        }


                template <class externT, class internT>
        codecvt_utf8<externT, internT>::~codecvt_utf8
                ()
        {}


                template <class externT, class internT>
        codecvt_utf8<externT, internT>&
        codecvt_utf8<externT, internT>::operator =
                (const codecvt_utf8<externT, internT>& ccvt)
        {
                        strictness = ccvt.strictness;
                        return *this;
        }


                template <class externT, class internT>
        std::codecvt_base::result
        codecvt_utf8<externT, internT>::do_out
                (codecvt_utf8<externT, internT>::state_type& state,
                const codecvt_utf8<externT, internT>::intern_type* from,
                const codecvt_utf8<externT, internT>::intern_type* from_end,
                const codecvt_utf8<externT, internT>::intern_type*& from_next,
                codecvt_utf8<externT, internT>::extern_type* to,
                codecvt_utf8<externT, internT>::extern_type* to_limit,
                codecvt_utf8<externT, internT>::extern_type*& to_next)
        const
        {
                        assert(from != NULL);
                        assert(from_end != NULL);
                        assert(from <= from_end);
                        assert(to != NULL);
                        assert(to_limit != NULL);
                        assert(to <= to_limit);

                        from_next = from;
                        to_next = to;
                        state = codecvt_utf8_state<internT>();
                        while (from_next < from_end && to_next < to_limit)
                        {
                                if (state.is_encoding_complete())
                                {
                                        if (*from_next < 0x80)
                                        {
                                                        *to_next = static_cast<extern_type>(*from_next);
                                                        state.encode(1, *from_next);
                                        }
                                        else if (*from_next < 0x800)
                                        {
                                                        *to_next = 0xc0;
                                                        *to_next |= static_cast<extern_type>(*from_next >> 6);
                                                        state.encode(2, *from_next);
                                        }
                                        else if (*from_next < 0x10000)
                                        {
                                                        *to_next = 0xe0;
                                                        *to_next |= static_cast<extern_type>(*from_next >> 12);
                                                        state.encode(3, *from_next);
                                        }
                                        else if (*from_next < 0x200000)
                                        {
                                                        *to_next = 0xf0;
                                                        *to_next |= static_cast<extern_type>(*from_next >> 18);
                                                        state.encode(4, *from_next);
                                        }
                                        else if (*from_next < 0x4000000)
                                        {
                                                        *to_next = 0xf8;
                                                        *to_next |= static_cast<extern_type>(*from_next >> 24);
                                                        state.encode(5, *from_next);
                                        }
                                        else if (*from_next <= 0x7fffffff)
                                        {
                                                        *to_next = 0xfc;
                                                        *to_next |= static_cast<extern_type>(*from_next >> 30);
                                                        state.encode(6, *from_next);
                                        } else
                                        {
                                                        return std::codecvt_base::error;
                                        }
                                }
                                else
                                {
                                                *to_next = 0x80;
                                                *to_next |= static_cast<extern_type>((state.unicode() >> (state.left_characters_to_encode() - 1)*6) & 0x3f);
                                                state.encoded();
                                }

                                to_next++;
                                if (state.left_characters_to_encode() == 0)
                                {
                                                from_next++;
                                }

                                assert(from <= from_next);
                                assert(from_next <= from_end);
                                assert(to <= to_next);
                                assert(to <= to_limit);
                        }

                        if (from_next == from_end)
                        {
                                        return std::codecvt_base::ok;
                        }
                        else
                        {
                                        return std::codecvt_base::partial;
                        }
        }
        
                        
                template <class externT, class internT>
        std::codecvt_base::result
        codecvt_utf8<externT, internT>::do_in
                (codecvt_utf8<externT, internT>::state_type& state,
                const codecvt_utf8<externT, internT>::extern_type* from,
                const codecvt_utf8<externT, internT>::extern_type* from_end,
                const codecvt_utf8<externT, internT>::extern_type*& from_next,
                codecvt_utf8<externT, internT>::intern_type* to,
                codecvt_utf8<externT, internT>::intern_type* to_limit,
                codecvt_utf8<externT, internT>::intern_type*& to_next)
        const
        {
                        assert(from != NULL);
                        assert(from_end != NULL);
                        assert(from <= from_end);
                        assert(to != NULL);
                        assert(to_limit != NULL);
                        assert(to <= to_limit);

                        from_next = from;
                        to_next = to;
                        while (from_next < from_end && to_next < to_limit)
                        {
                                if (state.is_decoding_complete())
                                {
                                        if ((*from_next & 0x80) == 0x00)
                                        {
                                                        *to_next = static_cast<intern_type>(*from_next);
                                                        state.decode(1);
                                        }
                                        else if ((*from_next & 0xe0) == 0xc0)
                                        {
                                                        *to_next = static_cast<intern_type>(*from_next & 0x1f) << 6;
                                                        state.decode(2);
                                        }
                                        else if ((*from_next & 0xf0) == 0xe0)
                                        {
                                                        *to_next = static_cast<intern_type>(*from_next & 0x0f) << 12;
                                                        state.decode(3);
                                        }
                                        else if ((*from_next & 0xf8) == 0xf0)
                                        {
                                                        *to_next = static_cast<intern_type>(*from_next & 0x07) << 18;
                                                        state.decode(4);
                                        }
                                        else if ((*from_next & 0xfc) == 0xf8)
                                        {
                                                        *to_next = static_cast<intern_type>(*from_next & 0x03) << 24;
                                                        state.decode(5);
                                        }
                                        else if ((*from_next & 0xfe) == 0xfc)
                                        {
                                                        *to_next = static_cast<intern_type>(*from_next & 0x01) << 30;
                                                        state.decode(6);
                                        }
                                        else
                                        {
                                                        return std::codecvt_base::error;
                                        }
                                }
                                else
                                {
                                        if ((*from_next & 0xc0) == 0x80)
                                        {
                                                        *to_next |= static_cast<intern_type>(*from_next & 0x3f) << (state.left_characters_to_decode() - 1)*6;
                                                        state.decoded();
                                        }
                                        else
                                        {
                                                        return std::codecvt_base::error;
                                        }
                                }

                                from_next++;
                                if (state.left_characters_to_decode() == 0)
                                {
                                                to_next++;
                                }

                                assert(from <= from_next);
                                assert(from_next <= from_end);
                                assert(to <= to_next);
                                assert(to_next <= to_limit);
                        }
                
                if (from_next == from_end)
                {
                                return std::codecvt_base::ok;
                }
                else
                {
                                return std::codecvt_base::partial;
                }
        }


                template <class externT, class internT>
        std::codecvt_base::result
        codecvt_utf8<externT, internT>::do_unshift
                (codecvt_utf8<externT, internT>::state_type& state,
                codecvt_utf8<externT, internT>::extern_type* to,
                codecvt_utf8<externT, internT>::extern_type* to_limit,
                codecvt_utf8<externT, internT>::extern_type*& to_next)
        const
        {
                        assert(to != NULL);
                        assert(to_limit != NULL);
                        assert(to <= to_limit);
                        assert(state.is_valid());

                        to_next = to;
                        while (to_next < to_limit && !state.is_encoding_complete())
                        {
                                        *to_next = 0x80;
                                        *to_next |= static_cast<extern_type>((state.unicode() >> (state.left_characters_to_encode() - 1)*6) & 0x3f);
                                        to_next++;
                                        state.encoded();
                        }

                        if (state.is_encoding_complete())
                        {
                                        return std::codecvt_base::ok;
                        }
                        else
                        {
                                        return std::codecvt_base::error;
                        }
        }

        
                template <class externT, class internT>
        int
        codecvt_utf8<externT, internT>::do_encoding
                ()
        const throw ()
        {
                        return -1;
        }


                template <class externT, class internT>
        bool
        codecvt_utf8<externT, internT>::do_always_noconv
                ()
        const throw ()
        {
                        return false;
        }


                template <class externT, class internT>
        int
        codecvt_utf8<externT, internT>::do_length
                (codecvt_utf8<externT, internT>::state_type& state,
                const codecvt_utf8<externT, internT>::extern_type* from,
                const codecvt_utf8<externT, internT>::extern_type* end,
                size_t max)
        const
        {
                        assert(from != NULL);
                        assert(end != NULL);
                        assert(from <= end);
                        assert(state.is_valid());

                        size_t size = 0;
                        if (state.left_characters_to_decode() > 0)
                        {
                                        size = 1;
                        }

                        const extern_type* next = from;
                        bool error = false;
                        while (next < end && size < max && !error)
                        {
                                if (state.is_decoding_complete())
                                {
                                        if ((*next & 0x80) == 0x00)
                                        {
                                                        state.decode(1);
                                        }
                                        else if ((*next & 0xe0) == 0x0c0)
                                        {
                                                        state.decode(2);
                                        }
                                        else if ((*next & 0xf0) == 0xe0)
                                        {
                                                        state.decode(3);
                                        }
                                        else if ((*next & 0xf8) == 0xf0)
                                        {
                                                        state.decode(4);
                                        }
                                        else if ((*next & 0xfc) == 0xf8)
                                        {
                                                        state.decode(5);
                                        }
                                        else if ((*next & 0xfe) == 0xfc)
                                        {
                                                        state.decode(6);
                                        }
                                        else
                                        {
                                                        error = true;
                                        }
                                }
                                else
                                {
                                        if ((*next & 0xc0) == 0x80)
                                        {
                                                        state.decoded();
                                        }
                                        else
                                        {
                                                        error = true;
                                        }
                                }

                                next++;
                                if (state.left_characters_to_decode() == 0)
                                {
                                        size++;
                                }
                        }
                        
                        if (error)
                        {
                                        return -1;
                        }
                        else
                        {
                                        return static_cast<int>(size);
                        }
        }


                template <class externT, class internT>
        int
        codecvt_utf8<externT, internT>::do_max_length
                ()
        const throw ()
        {
                        return 6;
        }
        
}

#endif
