﻿/**
 * @file GameTimeEventListener.h
 * @brief GameTimeEventListenerクラスのヘッダファイル
 * @author morimoto
 */
#ifndef GAMETIMEEVENTLISTENER_H
#define GAMETIMEEVENTLISTENER_H
#include <vector>
#include "..\EventListener.h"

/**
 * GameTimeEventのイベントメッセージを受け取るクラス
 */
class GameTimeEventListener : public EventListener
{
public:
	/**
	 * GameTimeEventListenerクラスのコンストラクタ
	 */
	GameTimeEventListener();

	/**
	 * GameTimeEventListenerクラスのデストラクタ
	 */
	virtual ~GameTimeEventListener();

	/**
	 * イベントメッセージを受け取る関数
	 * @param[in] _pEvent 受け取るEventクラスのポインタ
	 */
	virtual void EventMessage(Event* _pEvent);

	/**
	 * 最新のイベント情報を取得する関数
	 * @return 最新のイベントの種類
	 */
	int GetEventType();

	/**
	 * イベント情報を格納しているコンテナの中身を破棄する
	 */
	void ClearEventType();

private:
	std::vector<int> m_EventTypeContainer;

};


#endif
