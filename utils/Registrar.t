#ifndef REGISTRAR_T__
#define REGISTRAR_T__

#include <stdexcept>
#include <vector>
#include <map>

class DuplicateError : public std::runtime_error {
public:
	DuplicateError(const std::string& what) : std::runtime_error("Duplicate Error: " + what) {}
};

class NotFound : public std::runtime_error {
public:
	NotFound(const std::string& what) : std::runtime_error("Not found: " + what) {}
};

template< class T_id, class T_data, int unique=0 >
class Registrar {
/* unique can be used to make 2 registrar's with the same types */

	/* Singleton pattern */
public:
	static Registrar& get_instance() {
		static Registrar single_instance;
		return single_instance;
	}
	/* returns a reference to the only existing instance of this class */
private:
	Registrar() {};
	/* Private constructor */

public:
	void add( const T_id& id, const T_data& data ) throw(DuplicateError) {
	/* Register data under id */
		if( m_register.find(id) != m_register.end() ) {
			throw DuplicateError("");
		}
		m_register[id] = data;
	}

	std::vector< T_id > get_list() const throw() {
	/* Returns the list of ID's currently registered */
		std::vector< T_id > list;
		for( typeof(m_register.begin()) i = m_register.begin(); i != m_register.end(); i++ ) {
			list.push_back( i->first );
		}
		return list;
	}

	T_data get( const T_id& id ) const throw(NotFound) {
	/* Lookup the data for ID */
		typeof(m_register.begin()) i = m_register.find( id );
		if( i == m_register.end() ) throw NotFound("Registrar: couldn't find " + id);
		return i->second;
	}

private:
	std::map< T_id, T_data > m_register;
};

#endif /* REGISTRAR_T__ */
