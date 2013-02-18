/*
    Copyright 2007-2012 Christian Henning, Andreas Pokorny, Lubomir Bourdev
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_DETAIL_PNG_IO_SCANLINE_READ_HPP
#define BOOST_GIL_EXTENSION_IO_DETAIL_PNG_IO_SCANLINE_READ_HPP

////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief
/// \author Christian Henning, Andreas Pokorny, Lubomir Bourdev \n
///
/// \date   2007-2012 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include <boost/gil/gil_all.hpp>

#include <boost/gil/extension/io/detail/base.hpp>
#include <boost/gil/extension/io/detail/reader_base.hpp>
#include <boost/gil/extension/io/detail/conversion_policies.hpp>
#include <boost/gil/extension/io/detail/io_device.hpp>
#include <boost/gil/extension/io/detail/typedefs.hpp>
#include <boost/gil/extension/io/detail/row_buffer_helper.hpp>

#include "reader_backend.hpp"
#include "is_allowed.hpp"

namespace boost { namespace gil { 

///
/// PNG Reader
///
template< typename Device >
class scanline_reader< Device
                     , png_tag
                     >
    : public reader_backend< Device
                           , png_tag
                           >
{
private:

    typedef scanline_reader< Device
                           , png_tag
                           > this_t;

public:

    typedef reader_backend< Device, png_tag > backend_t;

public:

    //
    // Constructor
    //
    scanline_reader( const Device&                         io_dev
                   , const image_read_settings< png_tag >& settings
                   )
    : reader_backend< Device
                    , png_tag
                    >( io_dev
                     , settings
                     )
    {
        initialize();
    }

    void read( byte_t* dst
             , int
             )
    {
        read_scanline( dst );
    }

    /// Skip over a scanline.
    void skip( byte_t* dst, int )
    {
        read_scanline( dst );
    }

    void clean_up() {}

private:

    void initialize()
    {
        // Now it's time for some transformations.

        if( little_endian() )
        {
            if( this->_info._bit_depth == 16 )
            {
                // Swap bytes of 16 bit files to least significant byte first.
                png_set_swap( this->get()->_struct );
            }

            if( this->_info._bit_depth < 8 )
            {
                // swap bits of 1, 2, 4 bit packed pixel formats
                png_set_packswap( this->get()->_struct );
            }
        }

        if( this->_info._color_type == PNG_COLOR_TYPE_PALETTE )
        {
            png_set_palette_to_rgb( this->get()->_struct );
        }

        if( this->_info._num_trans > 0 )
        {
            png_set_tRNS_to_alpha( this->get()->_struct );
        }

        // Tell libpng to handle the gamma conversion for you.  The final call
        // is a good guess for PC generated images, but it should be configurable
        // by the user at run time by the user.  It is strongly suggested that
        // your application support gamma correction.
        if( this->_settings._apply_screen_gamma )
        {
            // png_set_gamma will change the image data!

#ifdef BOOST_GIL_IO_PNG_FLOATING_POINT_SUPPORTED
        png_set_gamma( this->get()->_struct
                     , this->_settings._screen_gamma
                     , this->_info._file_gamma
                     );
#else
        png_set_gamma( this->get()->_struct
                     , this->_settings._screen_gamma
                     , this->_info._file_gamma
                     );
#endif // BOOST_GIL_IO_PNG_FLOATING_POINT_SUPPORTED
        }

        // Interlaced images are not supported.
        this->_number_passes = png_set_interlace_handling( this->get()->_struct );
        io_error_if( this->_number_passes != 1
                   , "scanline_read_iterator cannot read interlaced png images."
                   );


        // The above transformation might have changed the bit_depth and color type.
        png_read_update_info( this->get()->_struct
                            , this->get()->_info
                            );

        this->_info._bit_depth = png_get_bit_depth( this->get()->_struct
                                                  , this->get()->_info
                                                  );

        this->_info._num_channels = png_get_channels( this->get()->_struct
                                                    , this->get()->_info
                                                    );

        this->_info._color_type = png_get_color_type( this->get()->_struct
                                                    , this->get()->_info
                                                    );

        this->_scanline_length = png_get_rowbytes( this->get()->_struct
                                                 , this->get()->_info
                                                 );
    }

    void read_scanline( byte_t* dst )
    {
        png_read_row( this->get()->_struct
                    , dst
                    , NULL
                    );
    }
};

} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_DETAIL_PNG_IO_SCANLINE_READ_HPP
