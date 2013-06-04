/**
 @file
 @author		Nikolaus Karpinsky
 */

#ifndef _PORTAL_CAPTURE_SCRIPT_INTERFACE_H_
#define _PORTAL_CAPTURE_SCRIPT_INTERFACE_H_

#include <QObject>
#include <QScriptEngine>
#include <QMetaType>
#include <QFile>
#include <QTextStream>

#include <memory>
#include <iostream>

#include "Utils.h"

using namespace std;

class ScriptInterface : public QObject
{
  Q_OBJECT

private:
  QScriptValue						m_global;

  QScriptEngine m_scriptEngine;
  
public:
  ScriptInterface( void );
  
  void PushThis( QString thisName );
  void PopThis( void );

  template <typename objectType>
	void		AddObjectType( QString name )
  {
	m_scriptEngine.globalObject( ).setProperty( name, 
	  m_scriptEngine.newFunction( ScriptInterface::CreateScriptableObject<objectType> ) );
  }

  template <typename objectType, typename arg0>
	void		AddObjectType( QString name )
  {
	m_scriptEngine.globalObject( ).setProperty( name, 
	  m_scriptEngine.newFunction( ScriptInterface::CreateScriptableObject<objectType, arg0> ) );
  }

  template <typename objectType, typename arg0, typename arg1>
	void		AddObjectType( QString name )
  {
	m_scriptEngine.globalObject( ).setProperty( name, 
	  m_scriptEngine.newFunction( ScriptInterface::CreateScriptableObject<objectType, arg0, arg1> ) );
  }

  template <typename objectType, typename arg0, typename arg1, typename arg2>
	void		AddObjectType( QString name )
  {
	m_scriptEngine.globalObject( ).setProperty( name, 
	  m_scriptEngine.newFunction( ScriptInterface::CreateScriptableObject<objectType, arg0, arg1, arg2> ) );
  }

  template <typename objectType>
	void RegisterMetaObjectType( void )
  { qScriptRegisterMetaType( &m_scriptEngine, ScriptInterface::Object2ScriptValue<objectType>, ScriptInterface::ScriptValue2Object<objectType> ); }

public slots:
  void			RunScript(QString filename);

private:
  template <typename objectType>
	static QScriptValue Object2ScriptValue(QScriptEngine* engine, objectType* const &in)
  { return engine->newQObject(in); }

  template <typename objectType>
	static void ScriptValue2Object(const QScriptValue &object, objectType* &out)
  { out = qobject_cast<objectType*>( object.toQObject( ) ); }

  template <typename objectType>
	static QScriptValue CreateScriptableObject( QScriptContext* context, QScriptEngine* engine )
  {
	return engine->newQObject( new objectType( ), QScriptEngine::AutoOwnership, QScriptEngine::AutoCreateDynamicProperties );
  }

  template <typename objectType, typename arg0>
	static QScriptValue CreateScriptableObject( QScriptContext* context, QScriptEngine* engine )
  {
  	arg0 argument0 = qscriptvalue_cast<arg0>( context->argument(0) );
	
	return engine->newQObject( new objectType( argument0 ), QScriptEngine::AutoOwnership, QScriptEngine::AutoCreateDynamicProperties );
  }

  template <typename objectType, typename arg0, typename arg1>
	static QScriptValue CreateScriptableObject( QScriptContext* context, QScriptEngine* engine )
  {
	arg0 argument0 = qscriptvalue_cast<arg0>( context->argument(0) );
	arg1 argument1 = qscriptvalue_cast<arg1>( context->argument(1) );

	return engine->newQObject( new objectType( argument0, argument1 ), QScriptEngine::AutoOwnership, QScriptEngine::AutoCreateDynamicProperties );
  }

  template <typename objectType, typename arg0, typename arg1, typename arg2>
	static QScriptValue CreateScriptableObject( QScriptContext* context, QScriptEngine* engine )
  {
	arg0 argument0 = qscriptvalue_cast<arg0>( context->argument(0) );
	arg1 argument1 = qscriptvalue_cast<arg1>( context->argument(1) );
	arg2 argument2 = qscriptvalue_cast<arg2>( context->argument(2) );

	return engine->newQObject( new objectType( argument0, argument1, argument2 ), QScriptEngine::AutoOwnership, QScriptEngine::AutoCreateDynamicProperties );
  }
};
#endif	// _PORTAL_CAPTURE_SCRIPT_INTERFACE_H_