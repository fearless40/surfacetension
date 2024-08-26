#pragma once


/* Temp
 Contract class: 
	Indicates to the user that they must not hold onto the item for longer than a frame.
	This means that the memory item contained within is never owned by the user. However
	it prevents needing to use shared pointer and its book keeping mechanisim. 
	This class literally does nothing. Temp items expire at the end of a frame. If data inside 
	needs to be kept longer than make sure and copy the data somewhere. 
*/
struct NoTag {};	//Indicates no other attributres to the data
struct Sorted {};	//Indicates the array must be sorted

template <class Item, class Tag = NoTag>
class Temp {
public:
	Item & get() const {
		return item;
	}

	Temp(Item & it) : item(it) {}

	Temp(Temp< const Item, Tag> & t) : item(t.item) {}


	operator Item & () const {
		return get();
	}

private:
	Item & item;
};

template <class Item, class Tag = NoTag> 
Temp<const Item, Tag> makeTempConst(Item & item) {
	const Item & it = item;
	return Temp<const Item, Tag>(it);
}