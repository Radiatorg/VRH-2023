#include "stdafx.h"

int _tmain(int argc, _TCHAR** argv)
{
	setlocale(LC_ALL, "Russian");
	Log::LOG log = Log::INITLOG;
	try
	{
		Parm::PARM parm = Parm::getparm(argc, argv);																																																																 																																																	
		log = Log::getlog(parm.log);																																																												 																																																							
		Log::WriteLine(log, L"Тест", L" Без ошибок", L"");																																																																							 																																										
		Log::WriteLog(log);																																																													 																																																									
		Log::WriteParm(log, parm);																																																												 																																																								
		In::IN in = In::getin(parm.in);																																																										 																																																									
		Log::WriteIn(log, in);																																																										 																																																											
		Lex::LEX lex = Lex::lexAnaliz(log, in); 																																																									 																																																																																																																																																																																																																																																																																																													
		std::cout << "Лексический анализ завершён без ошибок\n\n";
		*log.stream << "\nЛексический анализ завершён без ошибок\n\n";
		LT::showTable(lex.lexTable, log);
		IT::showTable(lex.idTable, log);
		IT::show(lex.idTable, parm);
		*log.stream << "--------------------------------------------------------------------------\n СИНТАКСИЧЕСКИЙ АНАЛИЗАТОР:\n";
		MFST_TRACE_STARTl(log);
		MFST::Mfst mfst(lex, GRB::getGreibach(), parm.trace);
		if (!mfst.start(log))
			throw ERROR_THROW(600);																																																																					
		mfst.savededucation();

		std::cout << "Синтаксический анализ завершён без ошибок\n";
		mfst.printrules(log);

		Sem::checkSemantic(lex, log);	
		Gen::CodeGeneration(lex, parm.out);
		std::cout << "\nСемантический анализ завершён без ошибок\n\n";
		*log.stream << "Семантический анализ завершён без ошибок\n\n";
		//system("start D:\\KPO\\VRH_project\\Debug\\CompilationOfGenCode.bat");
		Log::Close(log);
	}
	catch (Error::ERROR error)
	{
		Log::WriteError(log, error);
		std::cout << "Ошибка " << error.id << ": " << error.message << ", строка " << error.inext.line << ", позиция " << error.inext.col << std::endl;
		system("pause");
	}
	return 0;
}