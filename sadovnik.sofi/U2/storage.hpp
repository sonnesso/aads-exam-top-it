#ifndef STORAGE_HPP
#define STORAGE_HPP

#include "meeting.hpp"
#include "person.hpp"

#include <list.hpp>

namespace sadovnik
{

  struct storage_t
  {
    List< person_entry_t > persons;
    List< meeting_t > meetings;
  };

  void initStorage(storage_t & storage);
  void clearStorage(storage_t & storage);
  person_entry_t * findPerson(storage_t & storage, std::size_t id);
  const person_entry_t * findPerson(const storage_t & storage, std::size_t id);
  bool hasPerson(const storage_t & storage, std::size_t id);
  bool hasDescription(const storage_t & storage, std::size_t id);
  void ensurePerson(storage_t & storage, std::size_t id);
  void removePerson(storage_t & storage, std::size_t id);

}

#endif
