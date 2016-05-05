/*
 * test-database.cc
 *
 * Copyright (C) 2009 Tim Marston <tim@ed.am>
 *
 * This file is part of sqlite3cc (hereafter referred to as "this program").
 * See http://ed.am/dev/sqlite3cc for more information.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <sqlite3cc.h>
#include <boost/filesystem.hpp>


#define DBFILE "test.db"


int main()
{
	// delete any existing test database
	if( boost::filesystem::exists( DBFILE ) )
		boost::filesystem::remove( DBFILE );

	// open database
	sqlite::connection conn( DBFILE );

	std::cout << "SQLite threading mode: " << sqlite::threadsafe() << "\n";

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	// test basic statement, via command class
	sqlite::command c1( conn,
		"CREATE TABLE pets ( "
			"name TEXT PRIMARY KEY, "
			"age INTEGER "
		")" );
	c1.step();

	// test basic_statement::bind() calls, via command
	sqlite::command c2( conn, "INSERT INTO pets VALUES( ?, ? )" );
	c2.bind( 1, "billy" ); c2.bind( 2, 12 ); c2.exec();
	sqlite::command c3( conn, "INSERT INTO pets VALUES( ?6, ?9 )" );
	c3.bind( 9, 16 ); c3.bind( 6, "mopti" ); c3.exec();
	sqlite::command c6( conn, "INSERT INTO pets VALUES( ?, ? )" );
	c6.bind_null( 1 ); c6.bind( 2, 123 ); c6.exec();
	sqlite::command c7( conn, "INSERT INTO pets VALUES( :foo, :bar )" );
	c7.bind( ":foo", "foocat" ); c7.bind( ":bar", 22 ); c7.exec();

	// test command binding via stream operator
	sqlite::command( conn, "INSERT INTO pets VALUES( ?, ? )" ) <<
		"tessa" << 16 << sqlite::exec;
	sqlite::command c4( conn, "INSERT INTO pets VALUES( ?, ? )" );
	c4 << sqlite::null << sqlite::null << sqlite::set_index( 1 ) <<
		"tamara" << sqlite::exec;
	assert( c4.changes() == 1 );

	// test query binding via stream operator
	sqlite::query( conn, "SELECT * FROM pets WHERE name = ? OR age = ?" )
		<< "foo" << 16;
	sqlite::query( conn, "SELECT * FROM pets WHERE name = ? OR name = ? OR "
		"name = ? OR name = ?" ) << "foo" << sqlite::null << sqlite::null
		<< sqlite::set_index( 1 ) << "bar" << "baz";

	// test connection command and query factory methods
	conn.make_command( "PRAGMA user_version = 12")->step();
	assert( conn.make_query( "PRAGMA user_version" )->step()
		.column< int >( 0 ) == 12 );

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	// test basic queries
	sqlite::query q1( conn, "SELECT age, age + 3, name "
		"FROM pets WHERE NOT age IS NULL "
		"ORDER BY age ASC" );

	// test column count and names
	assert( q1.column_count() == 3 );
	assert( q1.column_name( 2 ) == "name" );

	// test rows
	unsigned int age;
	std::string name;
	q1.step().column( 0, age ); assert( age == 12 );
	name = q1.step().column< std::string >( 2 ); assert( name == "mopti" );

	// test row stream operator
	sqlite::row r1 = q1.step();
	r1 >> age; assert( age == 16 );
	r1 >> age; assert( age == 19 );
	r1 >> name; assert( name == "tessa" );
	q1.step() >> age >> sqlite::null >> name;
	assert( age == 22 && name == "foocat" );
	sqlite::row row = q1.step();
	row >> age >> sqlite::null >> sqlite::null; assert( age == 123 );
	row >> sqlite::set_index( 1 ) >> age; assert( age == 126 );

	// test NULL value handling
	assert( row.column_type( 2 ) == SQLITE_NULL );
	age = row.column< unsigned int >( 2 ); assert( age == 0 );
	row.column( 2, name ); assert( name == "" );

	// test end of results
	assert( !q1.step() );

	// test num results
	sqlite::query q3( conn, "SELECT name FROM pets WHERE age = 16" );
	assert( q3.num_results() == 2 );

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	// test query iterator
	sqlite::query q2( conn, "SELECT * FROM pets ORDER BY age DESC" );
	sqlite::query::iterator i1 = q2.begin();
	assert( i1 != q2.end() );

	// test resetting iterators
	unsigned int count = 0;
	for( sqlite::query::iterator i = q2.begin(); i != q2.end(); i++ )
	{
		// test iterator dereferencing and row numbers
		assert( count++ == i->row_number() );
		if( i->row_number() == 3 )
			assert( i->column< std::string >( 0 ) == "tessa" &&
				i->column< unsigned int >( 1 ) == 16 );
	}

//	for( sqlite::query::iterator i = q2.begin(); i != q2.end(); i++ ) {
//		for( unsigned int a = 0; a < q2.column_count(); a++ )
//			std::cout << q2.column_name( a ) << "[" <<
//			i->column< std::string >( a ) << "] ";
//		std::cout << "\n";
//	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	// test transactions
	{
		sqlite::transaction_guard< sqlite::deferred_transaction > t1( conn );
	}
	{
		sqlite::transaction_guard< sqlite::immediate_transaction > t1( conn );
	}
	{
		sqlite::transaction_guard< sqlite::exclusive_transaction > t1( conn );
	}
	{
		sqlite::transaction_guard< sqlite::recursive_transaction > t1( conn );
		sqlite::transaction_guard< sqlite::recursive_transaction > t2( conn );
	}

	// test transaction guard rollback
	{
		sqlite::transaction_guard< > t1( conn );
		conn.exec( "UPDATE pets SET age = 99" );
	}
	sqlite::query( conn, "SELECT age FROM pets ORDER BY age DESC LIMIT 1" )
		.step() >> age; assert( age == 123 );

	// test recursive transactions
	{
		sqlite::transaction_guard< sqlite::recursive_transaction > t1( conn );
		conn.exec( "UPDATE pets SET age = 66" );

		{
			sqlite::transaction_guard< sqlite::recursive_transaction > t2( conn );
			conn.exec( "UPDATE pets SET age = 1" );
		}

		sqlite::query( conn, "SELECT age FROM pets ORDER BY age DESC LIMIT 1" )
			.step() >> age; assert( age == 66 );
	}
	sqlite::query( conn, "SELECT age FROM pets ORDER BY age DESC LIMIT 1" )
		.step() >> age; assert( age == 123 );

	// ok
	return 0;
}
