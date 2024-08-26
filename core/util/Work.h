#pragma once
namespace Util
{
	namespace Work
	{
		struct WorkData
		{
			void * inData;
			void * outData;
		};

		typedef void(*WorkFunction)(WorkData data);

		struct WorkItem
		{
			WorkItem( WorkFunction f, void * inData, void * outData ) : func(f) {
				data.inData = inData;
				data.outData = outData;
			}

			WorkItem() { };

			WorkItem(WorkItem &  item) : func(item.func), data( item.data) {
			}
			
			WorkItem( WorkItem && item ) = default;
			
			WorkFunction func;
			WorkData	 data;
		};

		template <class InDataType, class OutDataType>
		WorkItem wiHelper(WorkFunction func, InDataType idt, OutDataType odt) {
			return WorkItem(func,
				reinterpret_cast<void*>(idt),
				reinterpret_cast<void*>(odt));
		}

	};
};