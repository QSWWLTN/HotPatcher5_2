// Fill out your copyright notice in the Description page of Project Settings.


#include "FlibShaderPipelineCacheHelper.h"
#include "FlibPatchParserHelper.h"
#include "HotPatcherLog.h"
#include "ShaderPipelineCache.h"
#include "RHIShaderFormatDefinitions.inl"
#include "HAL/IConsoleManager.h"
#include "Misc/EngineVersionComparison.h"

#if UE_VERSION_NEWER_THAN(5,1,1)
static FName ShaderPlatformToShaderFormatName(EShaderPlatform Platform)
{
	switch (Platform)
	{
	case SP_PCD3D_SM6:
		return NAME_PCD3D_SM6;
	case SP_PCD3D_SM5:
		return NAME_PCD3D_SM5;
	case SP_PCD3D_ES3_1:
		return NAME_PCD3D_ES3_1;

	case SP_OPENGL_PCES3_1:
		return NAME_GLSL_150_ES31;
	case SP_OPENGL_ES3_1_ANDROID:
		return NAME_GLSL_ES3_1_ANDROID;

	case SP_METAL:
		return NAME_SF_METAL;
	case SP_METAL_MRT:
		return NAME_SF_METAL_MRT;
	case SP_METAL_TVOS:
		return NAME_SF_METAL_TVOS;
	case SP_METAL_MRT_TVOS:
		return NAME_SF_METAL_MRT_TVOS;
	case SP_METAL_MRT_MAC:
		return NAME_SF_METAL_MRT_MAC;
	case SP_METAL_SM5:
		return NAME_SF_METAL_SM5;
	case SP_METAL_MACES3_1:
		return NAME_SF_METAL_MACES3_1;
	case SP_VULKAN_ES3_1_ANDROID:
		return NAME_VULKAN_ES3_1_ANDROID;
	case SP_VULKAN_PCES3_1:
		return NAME_VULKAN_ES3_1;
	case SP_VULKAN_SM5:
		return NAME_VULKAN_SM5;
	case SP_VULKAN_SM5_ANDROID:
		return NAME_VULKAN_SM5_ANDROID;
	case SP_D3D_ES3_1_HOLOLENS:
		return NAME_D3D_ES3_1_HOLOLENS;

	default:
		if (FStaticShaderPlatformNames::IsStaticPlatform(Platform))
		{
			return FStaticShaderPlatformNames::Get().GetShaderFormat(Platform);
		}
		else
		{
			checkf(0, TEXT("Unknown EShaderPlatform %d!"), (int32)Platform);
			return NAME_PCD3D_SM5;
		}
}
}
#endif

bool UFlibShaderPipelineCacheHelper::LoadShaderPipelineCache(const FString& Name)
{
	UE_LOG(LogHotPatcher,Display,TEXT("Load Shader pipeline cache %s for platform %d"),*Name,*ShaderPlatformToShaderFormatName(GMaxRHIShaderPlatform).ToString());
#if UE_VERSION_OLDER_THAN(5,1,0)
	return FShaderPipelineCache::OpenPipelineFileCache(Name,GMaxRHIShaderPlatform);
#else
	return FShaderPipelineCache::OpenPipelineFileCache(GMaxRHIShaderPlatform);
#endif
}

bool UFlibShaderPipelineCacheHelper::EnableShaderPipelineCache(bool bEnable)
{
	UE_LOG(LogHotPatcher,Display,TEXT("EnableShaderPipelineCache %s"),bEnable?TEXT("true"):TEXT("false"));
	auto Var =  IConsoleManager::Get().FindConsoleVariable(TEXT("r.ShaderPipelineCache.Enabled"));
	if(Var)
	{
		Var->Set( bEnable ? 1 : 0);
	}
	return !!Var;
}

bool UFlibShaderPipelineCacheHelper::SavePipelineFileCache(EPSOSaveMode Mode)
{
#if UE_VERSION_OLDER_THAN(5,1,0)
	return FShaderPipelineCache::SavePipelineFileCache((FPipelineFileCache::SaveMode)Mode);
#else
	return FShaderPipelineCache::SavePipelineFileCache((FPipelineFileCacheManager::SaveMode)Mode);
#endif
}

bool UFlibShaderPipelineCacheHelper::EnableLogPSO(bool bEnable)
{
	UE_LOG(LogHotPatcher,Display,TEXT("EnableLogPSO %s"),bEnable?TEXT("true"):TEXT("false"));
	auto Var =  IConsoleManager::Get().FindConsoleVariable(TEXT("r.ShaderPipelineCache.LogPSO"));
	if(Var)
	{
		Var->Set( bEnable ? 1 : 0);
	}
	return !!Var;
}

bool UFlibShaderPipelineCacheHelper::EnableSaveBoundPSOLog(bool bEnable)
{
	UE_LOG(LogHotPatcher,Display,TEXT("EnableSaveBoundPSOLog %s"),bEnable?TEXT("true"):TEXT("false"));
	auto Var =  IConsoleManager::Get().FindConsoleVariable(TEXT("r.ShaderPipelineCache.SaveBoundPSOLog"));
	if(Var)
	{
		Var->Set( bEnable ? 1 : 0);
	}
	return !!Var;
}

bool UFlibShaderPipelineCacheHelper::IsEnabledUsePSO()
{
	bool ret = false;
	auto Var =  IConsoleManager::Get().FindConsoleVariable(TEXT("r.ShaderPipelineCache.Enabled"));
	if(Var)
	{
		ret = !!Var->GetInt();
	}
	return ret;
}

bool UFlibShaderPipelineCacheHelper::IsEnabledLogPSO()
{
	bool ret = false;
	auto Var =  IConsoleManager::Get().FindConsoleVariable(TEXT("r.ShaderPipelineCache.LogPSO"));
	if(Var)
	{
		ret = !!Var->GetInt();
	}
	return ret;
}

bool UFlibShaderPipelineCacheHelper::IsEnabledSaveBoundPSOLog()
{
	bool ret = false;
	auto Var =  IConsoleManager::Get().FindConsoleVariable(TEXT("r.ShaderPipelineCache.SaveBoundPSOLog"));
	if(Var)
	{
		ret = !!Var->GetInt();
	}
	return ret;
}
