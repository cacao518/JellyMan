// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraShakeEffect.h"

UCameraShakeEffect::UCameraShakeEffect()
{
	// 지속시간
	OscillationDuration = 0.15f;

	// Y축 진폭, 주파수
	LocOscillation.Y.Amplitude = 20.f;
	LocOscillation.Y.Frequency = 100.f;

	// Z축 진폭, 주파수
	LocOscillation.Z.Amplitude = 20.f;
	LocOscillation.Z.Frequency = 100.f;

	// 카메라 시점 진동
	/*FOVOscillation.Amplitude = 1.f;
	FOVOscillation.Frequency = 100.f;*/

	// 최대 한번만 카메라쉐이크 -> 계속 호출 가능하면 어색
	bSingleInstance = true;
}