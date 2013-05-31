#include "ScriptInterface.h"

ScriptInterface::ScriptInterface( void )
{
  // Save the original global object so we can still search for
  // objects, even if the global object is changed.
  m_global = m_scriptEngine.globalObject( );
}

void ScriptInterface::PushThis( QString thisName )
{
  //  Find our object, then set it as 'this' in our new context
  auto thisObject = m_global.property( thisName );
  QScriptContext* context = m_scriptEngine.pushContext( );
  context->setThisObject( thisObject );
}

void ScriptInterface::PopThis( void )
{
  //  Pop the current context and restore the 'this' object
  m_scriptEngine.popContext( );
}

void ScriptInterface::RunScript(QString filename)
{
  QFile scriptFile(filename);
  if ( !scriptFile.open( QIODevice::ReadOnly ) )
  {
	//TODO: Log an error
	//wrench::Logger::logError( "Unable to load script file %s", filename.toLocal8Bit().data() ); 
	return;
  }
  QTextStream stream(&scriptFile);
  //  Not the most efficent way but it will work for now
  QString contents = stream.readAll();
  scriptFile.close();

  m_scriptEngine.evaluate(contents, filename).toString();
  if( m_scriptEngine.hasUncaughtException( ) )
  {
	int line = m_scriptEngine.uncaughtExceptionLineNumber();
	auto exception = m_scriptEngine.uncaughtException();
	//TODO: Log an error
	//wrench::Logger::logError( "Uncaught exception at line: %d %s", line, result.toString().toLocal8Bit().data() );
  }
}