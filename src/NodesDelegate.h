// https://github.com/CedricGuillemet/Imogen
//
// The MIT License(MIT)
// 
// Copyright(c) 2018 Cedric Guillemet
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#pragma once

#include "Nodes.h"
#include "Evaluation.h"
#include "ImCurveEdit.h"
#include "ImGradient.h"
#include "Library.h"
#include "EvaluationContext.h"

struct TileNodeEditGraphDelegate : public NodeGraphDelegate
{
	TileNodeEditGraphDelegate();

	void Clear();

	virtual void AddSingleNode(size_t type);
	virtual void UserAddNode(size_t type);
	virtual void AddLink(int InputIdx, int InputSlot, int OutputIdx, int OutputSlot) { gEvaluation.AddEvaluationInput(OutputIdx, OutputSlot, InputIdx);	}
	virtual void DelLink(int index, int slot) { gEvaluation.DelEvaluationInput(index, slot); }
	virtual void UserDeleteNode(size_t index);
	virtual void SetParamBlock(size_t index, const std::vector<unsigned char>& parameters);

	virtual unsigned int GetNodeTexture(size_t index) { return mEditingContext.GetEvaluationTexture(index); }

	void EditNode();

	virtual void SetTimeSlot(size_t index, int frameStart, int frameEnd);
	void SetTimeDuration(size_t index, int duration);
	void SetTime(int time, bool updateDecoder);
	size_t ComputeTimelineLength() const;

	virtual void DoForce();
	void InvalidateParameters();

	void SetMouse(float rx, float ry, float dx, float dy, bool lButDown, bool rButDown, float wheel);

	size_t ComputeNodeParametersSize(size_t nodeTypeIndex);
	bool NodeHasUI(size_t nodeIndex) { return gMetaNodes[mNodes[nodeIndex].mType].mbHasUI; }
	virtual bool NodeIsProcesing(size_t nodeIndex) { return mEditingContext.StageIsProcessing(nodeIndex); }
	virtual bool NodeIsCubemap(size_t nodeIndex);
	virtual bool NodeIs2D(size_t nodeIndex);
	virtual void UpdateEvaluationList(const std::vector<size_t> nodeOrderList) { gEvaluation.SetEvaluationOrder(nodeOrderList);	}
	virtual ImVec2 GetEvaluationSize(size_t nodeIndex);
	
	virtual void CopyNodes(const std::vector<size_t> nodes);
	virtual void CutNodes(const std::vector<size_t> nodes);
	virtual void PasteNodes();

	EvaluationContext mEditingContext;

	Mat4x4* GetParameterViewMatrix(size_t index) { if (index >= mNodes.size()) return NULL; return &mNodes[index].mParameterViewMatrix; }

	struct ImogenNode
	{
#ifdef _DEBUG
		std::string mNodeTypename;
#endif
		size_t mType;
		std::vector<unsigned char> mParameters;
		unsigned int mRuntimeUniqueId;
		int mStartFrame, mEndFrame;
		std::vector<InputSampler> mInputSamplers;

		Mat4x4 mParameterViewMatrix = Mat4x4::GetIdentity();
	};

	std::vector<ImogenNode> mNodes;
	std::vector<ImogenNode> mNodesClipboard;
	bool mbMouseDragging;

	ImogenNode* Get(ASyncId id) { return GetByAsyncId(id, mNodes); }
protected:
	void InitDefault(ImogenNode& node);
};

extern TileNodeEditGraphDelegate gNodeDelegate;