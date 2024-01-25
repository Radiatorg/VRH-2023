#include "stdafx.h"
#include <queue>

namespace Sem
{
	bool Sem::checkSemantic(Lex::LEX& tables, Log::LOG& log)
	{
		for (int i = 0; i < tables.lexTable.size; i++)
		{

			switch (tables.lexTable.table[i].lexema)
			{
			case LEX_PRINT:																																																																																																 																					 // проверка print
			{
				i++;
				if (tables.lexTable.table[i++].lexema == LEX_LEFTTHESIS)
					if (tables.lexTable.table[i].lexema == LEX_ID || tables.lexTable.table[i].lexema == LEX_LITERAL)
						break;
				throw ERROR_THROW_IN(509, tables.lexTable.table[i].numOfString, 0);
			}
			case LEX_LOGICAL:																																																																																									 																												 // проверка логический операторов
			{
				IT::IDDATATYPE dataTypeLeftOp = tables.idTable.table[tables.lexTable.table[i - 1].idxTI].idDataType;
				IT::IDDATATYPE dataTypeRightOp = tables.idTable.table[tables.lexTable.table[i + 1].idxTI].idDataType;

				if (dataTypeLeftOp != IT::INTEGER || dataTypeRightOp != IT::INTEGER)
					throw ERROR_THROW_IN(508, tables.lexTable.table[i].numOfString, 0);

				break;
			}
			case LEX_POW:																																																																																																										 											 // проверка pow 
			{
				int paramsCount = 0;

				for (int j = i + 2; tables.lexTable.table[j].lexema != LEX_RIGHTTHESIS; j++)
				{
					if (tables.lexTable.table[j].lexema == LEX_ID || tables.lexTable.table[j].lexema == LEX_LITERAL)
					{
						paramsCount++;
						IT::IDDATATYPE ctype = tables.idTable.table[tables.lexTable.table[j].idxTI].idDataType;
						if (ctype != IT::INTEGER)
							throw ERROR_THROW_IN(506, tables.lexTable.table[i].numOfString, 0);
					}
				}
				if (paramsCount != 2)
					throw ERROR_THROW_IN(506, tables.lexTable.table[i].numOfString, 0);
				break;
			}
			case LEX_ABS:																																																																																																													 								 // проверка compare
			{
				int paramsCount = 0;

				for (int j = i + 2; tables.lexTable.table[j].lexema != LEX_RIGHTTHESIS; j++)
				{
					if (tables.lexTable.table[j].lexema == LEX_ID || tables.lexTable.table[j].lexema == LEX_LITERAL)
					{
						paramsCount++;
						IT::IDDATATYPE ctype = tables.idTable.table[tables.lexTable.table[j].idxTI].idDataType;
						if (ctype != IT::INTEGER)
							throw ERROR_THROW_IN(507, tables.lexTable.table[i].numOfString, 0);
					}
				}
				if (paramsCount != 1)
					throw ERROR_THROW_IN(507, tables.lexTable.table[i].numOfString, 0);
				break;
			}
			case LEX_CYCLE:
			{
				int paramsCount = 0;
				for (int j = i + 2; tables.lexTable.table[j].lexema != LEX_RIGHTTHESIS; j++)
				{
					if (tables.lexTable.table[j].lexema == LEX_ID || tables.lexTable.table[j].lexema == LEX_LITERAL)
					{
						paramsCount++;
						IT::IDDATATYPE ctype = tables.idTable.table[tables.lexTable.table[j].idxTI].idDataType;
						if (ctype != IT::INTEGER)
							throw ERROR_THROW_IN(511, tables.lexTable.table[i].numOfString, 0);
					}
				}
				if (paramsCount != 1)
					throw ERROR_THROW_IN(511, tables.lexTable.table[i].numOfString, 0);
				break;

			}
			case LEX_EQUAL: {
				int position = i;
				if (tables.lexTable.table[position - 1].lexema == LEX_ID)
				{
					bool flag = false;
					int id_pos = position - 1;
					while (tables.lexTable.table[++position].lexema != LEX_SEMICOLON)
					{
						if (tables.lexTable.table[position].lexema == LEX_ID || tables.lexTable.table[position].lexema == LEX_LITERAL || tables.lexTable.table[position].lexema == LEX_ABS)
						{
							if (tables.idTable.table[tables.lexTable.table[position].idxTI].idType == IT::IDTYPE::F || tables.lexTable.table[position].lexema == LEX_ABS)
							{
								int counter = 0;
								for (int j = position;; j++)
								{
									position = j;
									if (tables.lexTable.table[j].lexema == LEX_LEFTTHESIS)
									{
										counter++;
									}
									if (tables.lexTable.table[j].lexema == LEX_RIGHTTHESIS)
									{
										if (counter == 1)
										{
											flag = true;
											break;
										}
										else
											counter--;

									}

								}


							}

							if (!flag && tables.lexTable.table[id_pos].idxTI != 0 && tables.idTable.table[tables.lexTable.table[id_pos].idxTI].idDataType != tables.idTable.table[tables.lexTable.table[position].idxTI].idDataType && tables.idTable.table[position].idType != IT::IDTYPE::F)
								throw ERROR_THROW_IN(512, tables.lexTable.table[i].numOfString, 0);

							if (!flag && tables.lexTable.table[id_pos].idxTI != 0 && tables.idTable.table[tables.lexTable.table[id_pos].idxTI].idDataType !=
								tables.idTable.table[tables.lexTable.table[position].idxTI].idDataType && tables.idTable.table[position].idType != IT::IDTYPE::F) {
								throw ERROR_THROW_IN(512, tables.lexTable.table[i].numOfString, 0);

							}
						}

					}

					break;
				}
			}
			case LEX_ID:
			{


				IT::Entry tmp = tables.idTable.table[tables.lexTable.table[i].idxTI];

				if (i > 0 && tables.lexTable.table[i - 1].lexema == LEX_FUNCTION)
				{
					if (tmp.idType == IT::F)
					{
						for (int k = i + 1; k != tables.lexTable.size; k++)
						{
							char l = tables.lexTable.table[k].lexema;
							if (l == LEX_RETURN)
							{
								int lexAfterReturn = tables.lexTable.table[k + 1].idxTI; 
								if (lexAfterReturn != TI_NULLIDX)
								{
									if (tables.idTable.table[lexAfterReturn].idDataType != tmp.idDataType)
									{
										throw ERROR_THROW_IN(502, tables.lexTable.table[k].numOfString, 0);
									}
								}
								break;
							}
						}
					}
				}
				if (tables.lexTable.table[i + 1].lexema == LEX_LEFTTHESIS && tables.lexTable.table[i - 1].lexema != LEX_FUNCTION) // вызов
				{
					if (tmp.idType == IT::F)
					{
						std::queue<LT::Entry> queue;

						int protoParamsCount = 0;

						for (int iterator = tmp.idxfirstLE + 1; tables.lexTable.table[iterator].lexema != LEX_RIGHTTHESIS; iterator++)
						{
							if (tables.lexTable.table[iterator].lexema == LEX_ID) {
								protoParamsCount++;
								queue.push(tables.lexTable.table[iterator]);
							}
						}
						if (protoParamsCount > PARAMS_MAX)
							throw ERROR_THROW_IN(503, tables.lexTable.table[i].numOfString, 0);


						for (int j = i + 1; tables.lexTable.table[j].lexema != LEX_RIGHTTHESIS; j++)
						{

							if (tables.lexTable.table[j].lexema == LEX_ID || tables.lexTable.table[j].lexema == LEX_LITERAL)
							{
								IT::IDDATATYPE ctype = tables.idTable.table[tables.lexTable.table[j].idxTI].idDataType;
								if (!queue.empty())
								{
									if (ctype != tables.idTable.table[queue.front().idxTI].idDataType)
									{

										throw ERROR_THROW_IN(504, tables.lexTable.table[i].numOfString, 0);
									}
									queue.pop();
								}
								else
								{

									throw ERROR_THROW_IN(505, tables.lexTable.table[i].numOfString, 0);
								}
							}
						}
						if (!queue.empty())
						{
							throw ERROR_THROW_IN(505, tables.lexTable.table[i].numOfString, 0);
						}
					}
				}
				char prevLex = tables.lexTable.table[i - 1].lexema;
				if (tmp.idxfirstLE == i && prevLex != LEX_INTEGER && prevLex != LEX_STRING && prevLex != LEX_FUNCTION)
					throw ERROR_THROW_IN(203, tables.lexTable.table[i].numOfString, 0);
				break;
			}

			}
			}
			return true;
		}
	}
