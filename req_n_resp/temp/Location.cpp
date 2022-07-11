#include "Location.hpp"

Location::Location() : index ("index.html"), root("/") {};

Location::~Location(){};

std::string Location::get_index()
{
	return(this->index);
}
