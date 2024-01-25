#pragma once
#define ID_MAXSIZE		8																																																																	
#define REGION_MAXSIZE	16																																																																	
#define TI_MAXSIZE		4096																																																																
#define TI_INT_DEFAULT	0x0000																																																																
#define TI_STR_DEFAULT	0x00																																																																
#define TI_NULLIDX		0x7FFFFFFF																																																																
#define TI_STR_MAXSIZE	255																																																																	
//2147483647

namespace IT																																																								 																																																																
{
	enum IDDATATYPE { INTEGER = 1, STR = 2 };			
	enum IDTYPE { V = 1, F = 2, P = 3, L = 4, OP = 5, LIB = 6 };
																																																																															

	struct Entry																																																							 																																																															
	{
		int			idxfirstLE;																																																																				
		unsigned char	idRegion[REGION_MAXSIZE];																																																															
		unsigned char	id[ID_MAXSIZE];																																																																		
		IDDATATYPE	idDataType;																																																																			
		IDTYPE		idType;																																																																					
		union
		{
			int vint;																																																																						
			struct
			{
				unsigned char len;																																																																				
				unsigned char str[TI_STR_MAXSIZE - 1];																																																															
			} vstr;																																																										 																																																															
		} value;																																																									 																																																														
	};

	struct IdTable																																																									 																																																																
	{
		int maxSize;																																																								 																																																																
		int size;																																																								 																																																																	
		Entry* table;																																																								 																																																																
	};

	IdTable Create(																																																								 																																																																	
		int size																																																							 																																																																		
	);

	void Add(																																																								 																																																																	
		IdTable& idTable,																																																									 																																																												
		Entry entry																																																									 																																																															
	);

	Entry GetEntry(																																																									 																																																															
		IdTable& idTable,																																																								 																																																													
		int n																																																									 																																																																
	);

	int IsIDRegion(																																																							 																																																																		
		IdTable& idTable,																																																								 																																																														
		unsigned char id[ID_MAXSIZE]																																																														
	);

	void Delete(IdTable& idTable);																																																															
	void showTable(IdTable& idTable, Log::LOG& log);
	void show(IdTable& idTable, Parm::PARM parm);
};