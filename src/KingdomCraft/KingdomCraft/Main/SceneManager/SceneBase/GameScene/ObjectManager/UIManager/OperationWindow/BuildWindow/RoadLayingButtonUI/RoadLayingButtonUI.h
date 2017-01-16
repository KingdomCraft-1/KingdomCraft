﻿/**
 * @file   RoadLayingButtonUI.h
 * @brief  RoadLayingButtonUIクラスのヘッダファイル
 * @author morimoto
 */
#ifndef ROADLAYINGBUTTONUI_H
#define ROADLAYINGBUTTONUI_H
#include "Vertex2D\Vertex2D.h"
#include "..\..\..\ButtonUI\ButtonUI.h"

/**
 * 道の敷設ボタンUIクラス
 */
class RoadLayingButtonUI : public ButtonUI
{
public:
	/**
	 * RoadLayingButtonUIクラスのコンストラクタ
	 * @param[in] _pParentUIPos 親UIの座標
	 * @param[in] _textureIndex 使用するテクスチャのインデックス番号
	 */
	RoadLayingButtonUI(const D3DXVECTOR2* _pParentUIPos, int _textureIndex);

	/**
	 * RoadLayingButtonUIクラスのデストラクタ
	 */
	virtual ~RoadLayingButtonUI();

	/**
	 * RoadLayingButtonUIクラスの制御関数
	 * @return ボタンが押されたらtrue
	 */
	virtual bool Control();

	/**
	 * RoadLayingButtonUIクラスの描画関数
	 */
	virtual void Draw();

private:
	RoadLayingButtonUI(const RoadLayingButtonUI&);
	void operator=(const RoadLayingButtonUI&);

	static const D3DXVECTOR2	m_DefaultPos;		//!< ボタンの初期座標
	static const D3DXVECTOR2	m_DefaultSize;		//!< ボタンの初期矩形サイズ
	static const D3DXVECTOR2	m_DefaultTexel[4];	//!< ボタンのテクスチャ初期座標
	Vertex2D*					m_pVertex2D;		//!< ボタンを描画するVertex2D
	int							m_TextureIndex;		//!< ボタンのテクスチャインデックス
	const D3DXVECTOR2*			m_pParentUIPos;		//!< 親UIの座標位置

};


#endif