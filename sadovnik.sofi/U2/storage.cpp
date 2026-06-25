#include "storage.hpp"

#include <utility>

namespace sadovnik
{

  void initStorage(storage_t & storage)
  {
    storage.persons.clear();
    storage.meetings.clear();
  }

  void clearStorage(storage_t & storage)
  {
    storage.persons.clear();
    storage.meetings.clear();
  }

  person_entry_t * findPerson(storage_t & storage, std::size_t id)
  {
    for (LIter< person_entry_t > it = storage.persons.begin();
      it != storage.persons.end();
      ++it)
    {
      if ((*it).id == id)
      {
        return &(*it);
      }
    }
    return nullptr;
  }

  const person_entry_t * findPerson(const storage_t & storage, std::size_t id)
  {
    for (LCIter< person_entry_t > it = storage.persons.cbegin();
      it != storage.persons.cend();
      ++it)
    {
      if ((*it).id == id)
      {
        return &(*it);
      }
    }
    return nullptr;
  }

  bool hasPerson(const storage_t & storage, std::size_t id)
  {
    return findPerson(storage, id) != nullptr;
  }

  bool hasDescription(const storage_t & storage, std::size_t id)
  {
    const person_entry_t * person = findPerson(storage, id);
    return person != nullptr && person->has_info;
  }

  void ensurePerson(storage_t & storage, std::size_t id)
  {
    if (hasPerson(storage, id))
    {
      return;
    }

    person_entry_t person;
    person.id = id;
    person.info.clear();
    person.has_info = false;
    storage.persons.pushBack(person);
  }

  void removePerson(storage_t & storage, std::size_t id)
  {
    List< person_entry_t > remaining;
    for (LCIter< person_entry_t > it = storage.persons.begin();
      it != storage.persons.end();
      ++it)
    {
      if ((*it).id != id)
      {
        remaining.pushBack(*it);
      }
    }
    storage.persons = std::move(remaining);
  }

}
