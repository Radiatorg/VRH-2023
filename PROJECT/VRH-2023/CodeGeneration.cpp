#include "stdafx.h"

namespace Gen
{
	void CodeGeneration(Lex::LEX& lex, wchar_t outfile[])
	{
		ofstream out(outfile);									
		if (!out.is_open())											
			throw ERROR_THROW(113);

		for (int i = 0; i < lex.lexTable.size && lex.lexTable.table[i].lexema != LEX_MAIN; i++)
		{

		}
		out << ".586\n\t.model flat, stdcall\n\tincludelib libucrt.lib\n\tincludelib kernel32.lib";
		out << "\n\tincludelib D:/KPO/VRH_project/Debug/lib_VRH" << "\n\n\t_printS PROTO :DWORD\n\t_printN PROTO :DWORD\n\t_pow PROTO :DWORD, :DWORD\n\t_abs PROTO :DWORD\n\t_pause PROTO ";
		out << "\n\tExitProcess PROTO :DWORD\n";
		out << "\n.stack 4096\n";

		out << "\n.const\n";
		for (int i = 0; i < lex.idTable.size; i++)
		{
			if (lex.idTable.table[i].idType == IT::L)
			{
				out << "\t" << lex.idTable.table[i].id;
				if (lex.idTable.table[i].idDataType == IT::STR)
				{	if(lex.idTable.table[i].value.vstr.str[1] == '\"')
						throw ERROR_THROW_IN(514, lex.lexTable.table[i].numOfString, 0);

					out << " BYTE '" << lex.idTable.table[i].value.vstr.str << "', 0\n";
				}
				if (lex.idTable.table[i].idDataType == IT::INTEGER)
				{
					out << " DWORD " << lex.idTable.table[i].value.vint << endl;
				}
			}
		}

		out << "\n.data\n";
		for (int i = 0; i < lex.lexTable.size; i++)
		{
			if (lex.lexTable.table[i].lexema == LEX_VAR)
			{
				out << "\t" << lex.idTable.table[lex.lexTable.table[i + 2].idxTI].idRegion;
				if (lex.idTable.table[lex.lexTable.table[i + 2].idxTI].idDataType == IT::STR)
				{
					out << " DWORD 0\n";
				}
				if (lex.idTable.table[lex.lexTable.table[i + 2].idxTI].idDataType == IT::INTEGER)
				{
					out << " DWORD 0\n";
				}
				i += 3;
			}
		}
		stack<char> words;
		stack<IT::Entry> stack;
		int iterop,
			numberOfPoints = 0,
			numberOfRet = 0,
			numberOfEnds = 0,
			itercycle = 1;
			
		string funcName = "";
		bool flagFunc = false,
			flagRet = false,
			flagMain = false,
			flagIf = false,
			flagThen = false,
			flagElse = false,
			flagCycle[5] = {},
			flagLibFunc = false;

		out << "\n.code\n";
		for (int i = 0; i < lex.lexTable.size; i++)
		{
			if (lex.lexTable.table[i].lexema == LEX_IF)
				words.push('c');
			if (lex.lexTable.table[i].lexema == LEX_ELSE)
				words.push('e');
			if (lex.lexTable.table[i].lexema == LEX_CYCLE)
				words.push('y');
			switch (lex.lexTable.table[i].lexema)
			{
			case LEX_FUNCTION:
			{
				funcName = (const char*)lex.idTable.table[lex.lexTable.table[++i].idxTI].idRegion;
				out << funcName << " PROC ";
				i += 2;
				for (; lex.lexTable.table[i].lexema != LEX_RIGHTTHESIS; i++)
				{
					if (lex.lexTable.table[i].lexema == LEX_ID || lex.lexTable.table[i].lexema == LEX_LITERAL)
					{
						if (lex.idTable.table[lex.lexTable.table[i].idxTI].idType == IT::P)
						{
							out << lex.idTable.table[lex.lexTable.table[i].idxTI].idRegion << " : ";
							if (lex.idTable.table[lex.lexTable.table[i].idxTI].idDataType == IT::INTEGER)
							{
								out << "DWORD";
							}
							else		// STR
							{

								out << "BYTE";
							}
						}
						if (lex.lexTable.table[i + 1].lexema == LEX_COMMA)
						{
							out << ", ";
							i++;
						}
					}
				}
				flagFunc = true;
				out << endl;
				break;
			}
			case LEX_MAIN:
			{
				flagMain = true;
				out << "\nmain PROC\n";
				break;
			}
			case LEX_EQUAL:
			{

				int result_position = i - 1;
				while (lex.lexTable.table[i].lexema != LEX_SEMICOLON)
				{

					switch (lex.lexTable.table[i].lexema)
					{
					case LEX_ABS:
					case LEX_POW:
					case LEX_ID:
					{
						IT::IDTYPE type = lex.idTable.table[lex.lexTable.table[i].idxTI].idType;
						if (type == IT::F || type == IT::LIB)
						{
							string tmpName;
							unsigned char lexema = lex.lexTable.table[i].lexema;

							switch (lexema)
							{
							case LEX_ABS:
							{
								tmpName = "_abs";
								i++;
								break;
							}
							case LEX_POW:
							{
								tmpName = "_pow";
								i++;
								break;
							}

							case LEX_ID:
							{
								tmpName = (const char*)lex.idTable.table[lex.lexTable.table[i++].idxTI].idRegion;
								break;	
							}
							}
							for (; lex.lexTable.table[i].lexema != LEX_RIGHTTHESIS; i++)
								if (lex.lexTable.table[i].lexema == LEX_ID || lex.lexTable.table[i].lexema == LEX_LITERAL) {
									stack.push(lex.idTable.table[lex.lexTable.table[i].idxTI]);
									
								}
							while (!stack.empty())
							{
								if (stack.top().idDataType == IT::INTEGER)
								{
									out << "\tmov eax, " << stack.top().idRegion << endl;
									out << "\tpush eax" << endl;
								}
								if (stack.top().idDataType == IT::STR)
								{
									if (stack.top().idType == IT::L)
										out << "\tpush offset " << stack.top().idRegion << endl;
									else
										out << "\tpush " << stack.top().idRegion << endl;
									
								}
								stack.top();
								stack.pop();
							}
							out << "\tcall " << tmpName << "\n\tpush eax\n";
							break;
						}
						if (lex.idTable.table[lex.lexTable.table[i].idxTI].idDataType == IT::INTEGER)
						{
							out << "\tpush " << lex.idTable.table[lex.lexTable.table[i].idxTI].idRegion << endl;
							break;
						}
						if (lex.idTable.table[lex.lexTable.table[i].idxTI].idDataType == IT::STR)
						{
							out << "\tpush " << lex.idTable.table[lex.lexTable.table[i].idxTI].idRegion << endl;
							break;
						}
						break;
					}
					case LEX_PLUS:
					{
						iterop= i+1;
						if (lex.idTable.table[lex.lexTable.table[iterop].idxTI].idDataType == IT::STR)
						{
							throw ERROR_THROW_IN(513, lex.lexTable.table[i].numOfString, 0);
						}
						i++;
						out << "\tpop eax\n";
						out << "\tpush " << lex.idTable.table[lex.lexTable.table[i].idxTI].idRegion << endl;
						out << "\tpop ebx\n";
						out << "\tadd eax, ebx\n\tpush eax\n";
						break;
					}
					case LEX_MINUS:
					{
						iterop = i + 1;
						if (lex.idTable.table[lex.lexTable.table[iterop].idxTI].idDataType == IT::STR)
						{
							throw ERROR_THROW_IN(513, lex.lexTable.table[i].numOfString, 0);
							out << "\tpush offset " << lex.idTable.table[lex.lexTable.table[i].idxTI].id << endl;
						}
						iterop = i - 1;
						if (lex.idTable.table[lex.lexTable.table[iterop].idxTI].idDataType == IT::STR)
						{
							throw ERROR_THROW_IN(513, lex.lexTable.table[i].numOfString, 0);
							out << "\tpush offset " << lex.idTable.table[lex.lexTable.table[i].idxTI].id << endl;
						}
						out << "\tpop ebx\n";
						i++;
						out << "\tpush " << lex.idTable.table[lex.lexTable.table[i].idxTI].idRegion << endl;
						out << "\tpop eax\n";
						out << "\tsub ebx, eax\n\tpush ebx\n";
						break;
					}
					case LEX_LITERAL:
					{
						if (lex.idTable.table[lex.lexTable.table[i].idxTI].idDataType == IT::INTEGER)
						{
							out << "\tpush " << lex.idTable.table[lex.lexTable.table[i].idxTI].id << endl;
						}
						if (lex.idTable.table[lex.lexTable.table[i].idxTI].idDataType == IT::STR)
						{
							out << "\tpush offset " << lex.idTable.table[lex.lexTable.table[i].idxTI].id << endl;
						}
						break;
					}
					}
					i++;
				}
				out << "\tpop " << lex.idTable.table[lex.lexTable.table[result_position].idxTI].idRegion << "\n";
				break;
			}
			case LEX_PLUS:
			{
				out << "\tpop eax\n\tpop ebx\n";
				out << "\tadd eax, ebx\n\tpush eax\n";
				break;
			}
			case LEX_MINUS:
			{
				out << "\tpop ebx\n\tpop eax\n";
				out << "\tsub eax, ebx\n\tpush eax\n";
				break;
			}
			case LEX_RETURN:
			{
				out << "\tpush ";
				i++;
				if (lex.idTable.table[lex.lexTable.table[i].idxTI].idType == IT::L)
				{
					out << lex.idTable.table[lex.lexTable.table[i++].idxTI].value.vint << endl;
				}
				else
				{
					out << lex.idTable.table[lex.lexTable.table[i++].idxTI].idRegion << endl;
				}
				if (flagFunc || flagMain)
				{
					flagRet = true;
				}
				break;
			}
			case LEX_RIGHTBRACE:
			{
				if ((flagMain && words.empty()) || (!flagFunc && flagMain && flagRet))
				{
					if (flagRet)
					{
						flagRet = false;
					}
						out << "\tcall _pause\n";
					out << "\tcall ExitProcess\nmain ENDP\nend main";
					break;
				}
				if (flagFunc)
				{
					if (flagRet)
					{
						out << "\tpop eax\n\tret\n";
						out << funcName << " ENDP\n\n";
						flagFunc = false;
						flagRet = false;
						itercycle++;
					}

				}
				if (flagThen && words.top() == 'c')
				{
					flagThen = false;
					if (flagElse)
					{
						out << "\tjmp ife" << numberOfEnds << endl;
						flagElse = false;
					}
					out << "p" << numberOfPoints++ << ":\n";
					itercycle++;
					words.pop();
				}
				if (flagElse && words.top() == 'e')
				{
					flagElse = false;
					out << "ife" << numberOfEnds++ << ":\n";
					itercycle++;
					words.pop();
				}
				if (flagCycle[--itercycle] && words.top() == 'y')
				{
					out << "pop ecx\npop eax\n";
					out << "loop " << "c" << itercycle << "\n";
					flagCycle[itercycle] = false;
					words.pop();
				}
				break;
			}

			
			case LEX_CYCLE:
			{
				flagCycle[itercycle] = true;
				out << "\tmov ecx, " << lex.idTable.table[lex.lexTable.table[i + 2].idxTI].idRegion << endl;
				out << "c" << itercycle << ":\n";
				out << "push eax\npush ecx\n";
				itercycle++;
				break;
			}
			case LEX_IF:
			{
				flagIf = true;

				break;
			}
			case LEX_LEFTTHESIS:
			{
				if (flagIf)
				{
					if (lex.lexTable.table[i + 2].lexema == LEX_LOGICAL)
					{
						out << "\tmov eax, " << lex.idTable.table[lex.lexTable.table[i + 1].idxTI].idRegion << endl;
						out << "\tcmp eax, " << lex.idTable.table[lex.lexTable.table[i + 3].idxTI].idRegion << endl;
						if ((string)(const char*)lex.idTable.table[lex.lexTable.table[i + 2].idxTI].id == SEM_GREAT)
						{
							out << "\tjg p" << numberOfPoints << endl;
							out << "\tjl p" << numberOfPoints + 1 << endl;
							out << "\tje p" << numberOfPoints + 1 << endl;
						}
						else if ((string)(const char*)lex.idTable.table[lex.lexTable.table[i + 2].idxTI].id == SEM_LESS)
						{
							out << "\tjl p" << numberOfPoints << endl;
							out << "\tjg p" << numberOfPoints + 1 << endl;
							out << "\tje p" << numberOfPoints + 1 << endl;
						}
						else if ((string)(const char*)lex.idTable.table[lex.lexTable.table[i + 2].idxTI].id == SEM_EQUAL)
						{
							out << "\tje p" << numberOfPoints << endl;
							out << "\tjg p" << numberOfPoints + 1 << endl;
							out << "\tjl p" << numberOfPoints + 1 << endl;
						}
						else if ((string)(const char*)lex.idTable.table[lex.lexTable.table[i + 2].idxTI].id == SEM_GREATEQUAL)
						{
							out << "\tje p" << numberOfPoints << endl;
							out << "\tjg p" << numberOfPoints << endl;
							out << "\tjl p" << numberOfPoints + 1 << endl;
						}
						else if ((string)(const char*)lex.idTable.table[lex.lexTable.table[i + 2].idxTI].id == SEM_LESSEQUAL)
						{
							out << "\tje p" << numberOfPoints << endl;
							out << "\tjl p" << numberOfPoints << endl;
							out << "\tjg p" << numberOfPoints + 1 << endl;
						}
						else if ((string)(const char*)lex.idTable.table[lex.lexTable.table[i + 2].idxTI].id == SEM_NOTEQUAL)
						{
							out << "\tjl p" << numberOfPoints << endl;
							out << "\tjg p" << numberOfPoints << endl;
							out << "\tje p" << numberOfPoints + 1 << endl;
						}

						int j = i;
						while (lex.lexTable.table[j++].lexema != LEX_RIGHTBRACE)
						{
							while (lex.lexTable.table[j].lexema == DIV)
								j++;
							if (lex.lexTable.table[j + 2].lexema == LEX_ELSE)
							{
								flagElse = true;
								break;
							}
						}
					}
					flagThen = true;
					out << "p" << numberOfPoints++ << ":\n";
					flagIf = false;
					break;
				}

				break;
			}

			case LEX_ELSE:
			{
				flagElse = true;
				break;
			}
			case LEX_PRINT:
			{
				if (lex.idTable.table[lex.lexTable.table[i + 2].idxTI].idDataType == IT::INTEGER)
				{
					out << "\tpush " << lex.idTable.table[lex.lexTable.table[i + 2].idxTI].idRegion << "\n\tcall _printN\n";
				}
				else
				{
					if (lex.idTable.table[lex.lexTable.table[i + 2].idxTI].idType == IT::L)
						out << "\tpush offset " << lex.idTable.table[lex.lexTable.table[i + 2].idxTI].id << "\n\tcall _printS\n";
					else
						out << "\tpush " << lex.idTable.table[lex.lexTable.table[i + 2].idxTI].idRegion << "\n\tcall _printS\n";
				}
				break;
			}
			}
		}
		out.close();
	}
}