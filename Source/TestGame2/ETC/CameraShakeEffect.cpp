// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraShakeEffect.h"

UCameraShakeEffect::UCameraShakeEffect()
{
	// ���ӽð�
	OscillationDuration = 0.15f;

	// Y�� ����, ���ļ�
	LocOscillation.Y.Amplitude = 20.f;
	LocOscillation.Y.Frequency = 100.f;

	// Z�� ����, ���ļ�
	LocOscillation.Z.Amplitude = 20.f;
	LocOscillation.Z.Frequency = 100.f;

	// ī�޶� ���� ����
	/*FOVOscillation.Amplitude = 1.f;
	FOVOscillation.Frequency = 100.f;*/

	// �ִ� �ѹ��� ī�޶���ũ -> ��� ȣ�� �����ϸ� ���
	bSingleInstance = true;
}