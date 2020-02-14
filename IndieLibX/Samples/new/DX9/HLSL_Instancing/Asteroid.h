//-----------------------------------------------------------------------------
// Copyright NVIDIA Corporation 2004
// TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW, THIS SOFTWARE IS PROVIDED 
// *AS IS* AND NVIDIA AND ITS SUPPLIERS DISCLAIM ALL WARRANTIES, EITHER EXPRESS 
// OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF 
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL 
// NVIDIA OR ITS SUPPLIERS BE LIABLE FOR ANY SPECIAL, INCIDENTAL, INDIRECT, OR 
// CONSEQUENTIAL DAMAGES WHATSOEVER INCLUDING, WITHOUT LIMITATION, DAMAGES FOR 
// LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION, LOSS OF BUSINESS 
// INFORMATION, OR ANY OTHER PECUNIARY LOSS) ARISING OUT OF THE USE OF OR 
// INABILITY TO USE THIS SOFTWARE, EVEN IF NVIDIA HAS BEEN ADVISED OF THE 
// POSSIBILITY OF SUCH DAMAGES.
// 
// File: Asteroid.h
// Desc: The asteroid is a mega small struct that i used by the asteroid manager
//-----------------------------------------------------------------------------
#pragma once

struct Asteroid
{
public:
	Asteroid();
	void MakeRandomPosition(D3DXVECTOR3 &worldMin,D3DXVECTOR3 &worldMax);

	D3DXVECTOR3 position;
	D3DXQUATERNION rotation;
	D3DXVECTOR3 rotationAxis;
	D3DXVECTOR3 scale;	

	float radius;
	float radiusSq;
	D3DXCOLOR color;
	float speed;
	int quadrant;
};