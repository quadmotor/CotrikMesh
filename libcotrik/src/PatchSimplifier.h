#ifndef PATCH_SIMPLIFIER_H
#define PATCH_SIMPLIFIER_H

#include "Simplifier.h"
#include "AngleBasedSmoothQuadMesh.h"
class PatchSimplifier : public Simplifier {
public:
	PatchSimplifier(Mesh& mesh);
	virtual ~PatchSimplifier();
private:
	PatchSimplifier();
	PatchSimplifier(const PatchSimplifier&);
	PatchSimplifier& operator = (const PatchSimplifier&);
public:
	void Run();
	bool Simplify(int& iter);
	bool CheckCorners();
	SmoothAlgorithm* smoothing_algorithm;
};

#endif // !PATCH_SIMPLIFIER_H
