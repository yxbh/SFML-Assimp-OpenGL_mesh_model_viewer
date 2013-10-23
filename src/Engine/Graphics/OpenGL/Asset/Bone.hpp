#pragma once
#include "../../../common.hpp"
#include "../Maths.hpp"

namespace KG
{

	struct BoneTransform
	{
		glm::dmat4 offset;
		glm::dmat4 final;
	};

	typedef Vec4i VertexIDs;
	typedef Vec4i BoneIDs;
	typedef Vec4f BoneWeights;

	struct Skeleton
	{
		std::vector<std::string>	names;				// bone names
		std::vector<float>			weights;			// positon would be index to vertex. value is the weight.
		std::vector<BoneTransform>	bone_transforms;	// offset and final transform for each bone.
		
		void Reserve(const unsigned p_Size)
		{
			names.reserve(p_Size);
			weights.reserve(p_Size);
			bone_transforms.reserve(p_Size);
		}

	};


	typedef std::shared_ptr<KG::Skeleton>	Skeleton_SmartPtr;
	typedef std::weak_ptr<KG::Skeleton>		Skeleton_WeakPtr;
} // KG ns
