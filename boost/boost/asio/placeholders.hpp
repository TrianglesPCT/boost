//
// placeholders.hpp
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2006 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_PLACEHOLDERS_HPP
#define BOOST_ASIO_PLACEHOLDERS_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <boost/asio/detail/push_options.hpp>

#include <boost/asio/detail/push_options.hpp>
#include <boost/bind/arg.hpp>
#include <boost/detail/workaround.hpp>
#include <boost/asio/detail/pop_options.hpp>

namespace boost {
namespace asio {
namespace placeholders {

#if defined(GENERATING_DOCUMENTATION)

/// An argument placeholder, for use with @ref boost_bind, that corresponds to
/// the error argument of a handler for any of the asynchronous functions.
unspecified error;

/// An argument placeholder, for use with @ref boost_bind, that corresponds to
/// the bytes_transferred argument of a handler for asynchronous functions such
/// as boost::asio::basic_stream_socket::async_write_some or
/// boost::asio::async_write.
unspecified bytes_transferred;

/// An argument placeholder, for use with @ref boost_bind, that corresponds to
/// the iterator argument of a handler for asynchronous functions such as
/// boost::asio::basic_resolver::resolve.
unspecified iterator;

#elif defined(__BORLANDC__) || defined(__GNUC__)

inline boost::arg<1> error()
{
  return boost::arg<1>();
}

inline boost::arg<2> bytes_transferred()
{
  return boost::arg<2>();
}

inline boost::arg<2> iterator()
{
  return boost::arg<2>();
}

#else

namespace detail
{
  template <int Number>
  struct placeholder
  {
    static boost::arg<Number>& get()
    {
      static boost::arg<Number> result;
      return result;
    }
  };
}

#if BOOST_WORKAROUND(BOOST_MSVC, < 1300)

static boost::arg<1>& error
  = boost::asio::placeholders::detail::placeholder<1>::get();
static boost::arg<2>& bytes_transferred
  = boost::asio::placeholders::detail::placeholder<2>::get();
static boost::arg<2>& iterator
  = boost::asio::placeholders::detail::placeholder<2>::get();

#else

namespace
{
  boost::arg<1>& error
    = boost::asio::placeholders::detail::placeholder<1>::get();
  boost::arg<2>& bytes_transferred
    = boost::asio::placeholders::detail::placeholder<2>::get();
  boost::arg<2>& iterator
    = boost::asio::placeholders::detail::placeholder<2>::get();
} // namespace

#endif

#endif

} // namespace placeholders
} // namespace asio
} // namespace boost

#include <boost/asio/detail/pop_options.hpp>

#endif // BOOST_ASIO_PLACEHOLDERS_HPP
