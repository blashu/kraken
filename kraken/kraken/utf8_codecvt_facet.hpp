// Copyright © 2001 Ronald Garcia, Indiana University (garcia@osl.iu.edu)
// Andrew Lumsdaine, Indiana University (lums@osl.iu.edu). Permission to copy, 
// use, modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided "as is"
// without express or implied warranty, and with no claim as to its suitability
// for any purpose.

#ifndef SKYPE_LAUNCHER_UTF8_CODECVT_FACET_HPP
#define SKYPE_LAUNCHER_UTF8_CODECVT_FACET_HPP

#include <boost/config.hpp>

#define BOOST_UTF8_BEGIN_NAMESPACE \
     namespace skype_launcher {

#define BOOST_UTF8_END_NAMESPACE }
#define BOOST_UTF8_DECL

#include <boost/detail/utf8_codecvt_facet.hpp>

#undef BOOST_UTF8_BEGIN_NAMESPACE
#undef BOOST_UTF8_END_NAMESPACE
#undef BOOST_UTF8_DECL

#endif