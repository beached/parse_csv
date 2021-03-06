// The MIT License (MIT)
//
// Copyright (c) 2013-2016 Darrell Wright
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files( the "Software" ), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <boost/utility/string_view.hpp>
#include <functional>
#include <string>
#include <vector>

#include "data_cell.h"
#include "data_types.h"

#if USE_PPL == 1
#include <ppl.h>
#else
#ifdef _WIN32
#include <omp.h>
#endif
#endif


namespace daw {
	namespace data {
		template<typename StorageType>
		class DataColumn final {
		public:
			using value_type = typename StorageType::value_type;
			using reference = value_type&;
			using const_reference = const value_type&;
			using values_type = StorageType;
			using iterator = typename values_type::iterator;
			using const_iterator = typename values_type::const_iterator;
			using reverse_iterator = typename values_type::reverse_iterator;
			using const_reverse_iterator = typename values_type::const_reverse_iterator;
			using difference_type = typename values_type::difference_type;
			using size_type = typename values_type::size_type;
		protected:
			values_type m_items;
			std::string m_header;
			bool m_hidden;

			reference item( const size_type pos ) {
				return m_items[pos];
			}

			const_reference item( const size_type pos ) const {
				return m_items[pos];
			}

		public:
			DataColumn( std::string header = "" ) noexcept:
					m_items{ },
					m_header{ std::move( header ) }, 
					m_hidden{ false } { }

			~DataColumn( ) = default;


			bool operator==( DataColumn const & ) const = delete;

			DataColumn( DataColumn const & ) = default;
			DataColumn & operator=( DataColumn const & ) = default;

			DataColumn( DataColumn && other ) noexcept: 
				m_items{ std::move( other.m_items ) }, 
				m_header{ std::move( other.m_header ) }, 
				m_hidden{ std::move( other.m_hidden ) } { }

			friend void swap( DataColumn & lhs, DataColumn & rhs ) noexcept {
				using std::swap;
				swap( lhs.m_items, rhs.m_items );
				swap( lhs.m_header, rhs.m_header );
				swap( lhs.m_hidden, rhs.m_hidden );
			}

			DataColumn& operator=( DataColumn && rhs ) noexcept {
				if( this != &rhs ) {
					DataColumn tmp{ std::move( rhs ) };
					using std::swap;
					swap( *this, tmp );
				}
				return *this;
			}
	
			void shrink_to_fit( ) {
				shrink_to_fit( m_items );
			}

			template<typename Container>
			inline static void shrink_to_fit( Container & values ) {
				values.shrink_to_fit( );
			}

			void append( value_type value ) {
				m_items.push_back( std::move( value ) );
			}

			iterator erase( iterator first ) {
				auto ret = m_items.erase( first );
				return ret;
			}

			iterator erase( iterator first, iterator last ) {
				auto ret = m_items.erase( first, last );
				return ret;
			}

			void erase_item( size_type pos ) {
				m_items.erase( m_items.begin( ) + static_cast<difference_type>(pos) );
			}

			std::string const & header( ) const {
				return m_header;
			}

			std::string & header( ) noexcept {
				return m_header;
			}

			bool & hidden( ) noexcept {
				return m_hidden;
			}

			bool const & hidden( ) const noexcept {
				return m_hidden;
			}

			reference operator[]( size_type pos ) {
				return item( pos );
			}

			const_reference operator[]( size_type pos ) const {
				return item( pos );
			}

			iterator begin( ) {
				return m_items.begin( );
			}

			iterator end( ) {
				return m_items.end( );
			}

			const_iterator begin( ) const {
				return m_items.begin( );
			}

			const_iterator end( ) const {
				return m_items.end( );
			}

			const_iterator cbegin( ) const {
				return m_items.cbegin( );
			}

			const_iterator cend( ) const {
				return m_items.cend( );
			}

			reverse_iterator rbegin( ) {
				return m_items.rbegin( );
			}

			reverse_iterator rend( ) {
				return m_items.rend( );
			}

			const_reverse_iterator rbegin( ) const {
				return m_items.rbegin( );
			}

			const_reverse_iterator rend( ) const {
				return m_items.rend( );
			}

			const_reverse_iterator crbegin( ) const {
				return m_items.crbegin( );
			}

			const_reverse_iterator crend( ) const {
				return m_items.crend( );
			}

			size_type size( ) const noexcept {
				return m_items.size( );
			}

			bool empty( ) const noexcept {
				return m_items.empty( );
			}

			void clear( ) {
				m_items.clear( );
			}
		};	// DataColumn

		void convert_column_to_timestamp( DataColumn<std::vector<DataCell>> & column, bool is_nullable = true, boost::string_view format = "%d/%m/%y %H:%M:%S" );
	}	// namespace data
}	// namespace daw
