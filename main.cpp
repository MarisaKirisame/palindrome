#include <iostream>
#include <string>
#include <set>
#include <exception>
#include <cassert>
#include <algorithm>
#include <list>
#include <thread>
bool is_palindrome( const std::string & str )
{ return std::equal( str.begin( ), str.end( ), str.rbegin( ) ); }
bool can_be_palindrome( const std::string & str )
{ return std::equal( str.begin( ), str.end( ), str.rbegin( ), []( char a, char b ){ return a == '*' || b == '*' || a == b; } ); }
std::set< std::string > generate_palindrome( const std::string & str, const std::set< std::string > & dictionary )
{
    if ( ! can_be_palindrome( str ) ) { return { }; }
    if ( str.find( '*' ) == str.npos )
    {
        if ( is_palindrome( str ) ) { return { str }; }
        else { return { }; }
    }
    else
    {
        if ( std::equal( str.begin( ), str.end( ), str.rbegin( ) ) )
        {
            std::set< std::string > ret;
            for ( const std::string & insert_key : dictionary )
            {
                size_t pos = -1;
                pos = str.find( std::string( insert_key.length( ), '*' ).c_str( ), pos + 1, insert_key.length( ) );
                if ( pos == str.npos ) { continue; }
                std::string new_string( str );
                std::copy( insert_key.begin( ), insert_key.end( ), new_string.begin( ) + pos );
                auto res = generate_palindrome( new_string, dictionary );
                std::copy( res.begin( ), res.end( ), std::insert_iterator< std::set< std::string > >( ret, ret.end( ) ) );
            }
            return ret;
        }
        else
        {
            auto i = str.begin( );
            auto ii = str.rbegin( );
            while ( i != str.end( ) )
            {
                if ( * i != * ii ) { goto http; }
                ++i;
                ++ii;
            }
            throw std::logic_error( "string is equalized. no need to equalize it." );
            http://stackoverflow.com
            if ( * i != '*' && * ii != '*' ) { return { }; }
            char c;
            if ( * i == '*' ) { c = * ii; }
            else
            {
                assert( * ii == '*' );
                c = * i;
            }
            std::set< std::string > ret;
            for ( const std::string & insert_key : dictionary )
            {
                size_t pos = -1;
                while ( true )
                {
                    pos = insert_key.find_first_of( c, pos + 1 );
                    if ( pos == str.npos ) { break; }
                    if ( * i == '*' )
                    {
                        if ( static_cast< size_t >( std::distance( str.begin( ), i ) ) < pos ) { continue; }
                        if ( static_cast< size_t >( std::distance( i - pos, str.end( ) ) ) < insert_key.size( ) ) { continue; }
                        if ( std::all_of( i - pos, i - pos + insert_key.size( ), []( char ch ){ return ch == '*'; } ) )
                        {
                            std::string new_string( str );
                            std::copy( insert_key.begin( ), insert_key.end( ), new_string.begin( ) + std::distance( str.begin( ), i ) - pos );
                            auto res = generate_palindrome( new_string, dictionary );
                            std::copy( res.begin( ), res.end( ), std::insert_iterator< std::set< std::string > >( ret, ret.end( ) ) );
                        }
                    }
                    else
                    {
                        assert( * ii == '*' );
                        if ( static_cast< size_t >( std::distance( ii, str.rend( ) ) - 1 ) < pos ) { continue; }
                        if ( static_cast< size_t >( std::distance( str.rbegin( ), ii + pos ) + 1 ) < insert_key.size( ) ) { continue; }
                        if ( std::all_of( ii + pos + 1 - insert_key.size( ), ii + pos + 1, []( char ch ){ return ch == '*'; } ) )
                        {
                            std::string new_string( str );
                            std::copy(
                                        insert_key.begin( ),
                                        insert_key.end( ),
                                        new_string.begin( ) + ( new_string.size( ) - std::distance( str.begin( ), i ) - 1 ) - pos );
                            auto res = generate_palindrome( new_string, dictionary );
                            std::copy( res.begin( ), res.end( ), std::insert_iterator< std::set< std::string > >( ret, ret.end( ) ) );
                        }
                    }
                }
            }
            return ret;
        }
    }
}
std::set< std::string > generate_palindrome( size_t length, const std::set< std::string > & dictionary )
{ return generate_palindrome( std::string( length, '*' ), dictionary ); }
static const std::set< std::string > & test_dic( )
{
    static std::set< std::string > ret =
    {
        "a",
        "man",
        "plan",
        "cat",
        "canal",
        "panama",
        "ham",
        "yat",
        "hat",
        "pasta",
        "canoe",
        "heros",
        "rajahs",
        "coloratura",
        "maps",
        "snipe",
        "percale",
        "macaroni",
        "gag",
        "banana",
        "bag",
        "tan",
        "tag",
        "again",
        "camel",
        "crepe",
        "pins",
        "spam",
        "rut",
        "rolo",
        "cash",
        "jar",
        "sore",
        "hats",
        "peon"
    };
    return ret;
}
void display( std::set< std::string > ** get )
{
    std::list< std::set< std::string > > data;
    while ( true )
    {
        if ( * get != nullptr )
        {
            data.push_back( ** get );
            * get = nullptr;
        }
        if ( ! data.empty( ) )
        {
            auto & res = data.front( );
            for ( auto & i : res )
            {
                if ( * get != nullptr )
                {
                    data.push_back( ** get );
                    * get = nullptr;
                }
                std::cout << i << std::endl;
                std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
            }
            data.pop_front( );
        }
    }
}
int main()
{
    assert( is_palindrome( "amanaplanacatacanalpanama" ) );
    assert( ! is_palindrome( "this is not a palindrome" ) );
    std::set< std::string > * ptr = nullptr;
    std::thread t( display, & ptr );
    for ( int i = 0;; ++i )
    {
        auto res = generate_palindrome( i, test_dic( ) );
        ptr = & res;
        while ( ptr != nullptr ) { std::this_thread::yield( ); }
    }
}

