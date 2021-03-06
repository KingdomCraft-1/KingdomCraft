﻿/**
 * @file   ButtonUI.h
 * @brief  ButtonUIクラスのヘッダファイル
 * @author morimoto
 */
#ifndef BUTTONUI_H
#define BUTTONUI_H
#include "Vertex2D\Vertex2D.h"

/**
 * UIのボタンクラス
 */
class ButtonUI
{
public:
	/**
	 * ButtonUIクラスのコンストラクタ
	 * @param[in] _pButtonPos ボタンの描画位置
	 * @param[in] _pButtonVertex ボタンのサイズ
	 * @param[in] _isVisible ボタンの可視設定
	 */
	ButtonUI(const D3DXVECTOR2* _pButtonPos, const D3DXVECTOR2* _pButtonSize, bool _isVisible = true);

	/**
	 * ButtonUIクラスのデストラクタ
	 */
	virtual ~ButtonUI();

	/**
	 * ButtonUIクラスの制御関数
	 * @return ボタンが押されたらtrue
	 */
	virtual bool Control();

	/**
	 * ButtonUIクラスの描画関数
	 */
	virtual void Draw() = 0;

	/**
	 * ボタンの可視設定を行う関数
	 *
	 * falseを指定した場合はControl()とDraw()は処理を行わない\n
	 * trueを指定した場合は通常通り動作
	 * @param[in] _isVisible 可視設定するかのフラグ
	 */
	inline void SetVisible(bool _isVisible)
	{
		m_IsVisible = _isVisible;
	}

protected:
	/**
	 * クリックされたかをチェックする関数
	 * @todo 関数名が気に入らないから変更予定
	 */
	bool IsClicked();

	D3DXVECTOR2		m_Pos;			//!< ボタンの座標
	D3DXVECTOR2 	m_Size;			//!< ボタンのサイズ
	bool			m_IsMouseOver;	//!< マウスオーバー状態か
	bool			m_IsVisible;	//!< ボタンが可視状態か

};


#endif
