
template <class HandleType, class TagType = uint32_t>
class TagArray
{
private:
	struct Tag 
	{
		TagType mTag;
		HandleType mItem;
	};
	
	std::vector<Tag> Tags;

public:

	void add( TagType tag, HandleType handle ) 
	{
		Tag tg = { tag, handle };
		Tags.push_back( tg ); 
	}

	void removeAllTags()
	{
		Tags.clear();
	}

	void removeItemByTag( TagType tag, std::function<void( HandleType )> remover )
	{
		std::vector<Tag> tags;
		tags.reserve( Tags.size() );

		for( auto & item : Tags )
		{
			if( item.mTag == tag )
			{
				remover( item.mItem );
			}
			else
			{
				tags.push_back( item );
			}
		}

		Tags = std::move( tags );
	}

};