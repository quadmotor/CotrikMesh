/*
 * MeshProject.cpp
 *
 *  Created on: Jan 02, 2017
 *      Author: cotrik
 */

#include "MeshFileReader.h"
#include "MeshFileWriter.h"
#include "FeatureLine.h"
#include "ArgumentManager.h"
#include <iostream>
#include <vector>

void GetArguments(ArgumentManager& argumentManager,
    std::string& orig,// = "tri.vtk";
    std::string& input,// = "quad.vtk";
    std::string& output,// = "out.vtk";
    size_t& iters, // = 1;
    bool& preserveQuality, // = false;
    bool& preserveSharpFeature, // = false
    bool& treatSharpFeatureAsRegular,// = true
    bool& treatCornerAsRegular, // = false
    double& cosangle// = 0.939692621
)
{   const std::string strOrigTriFilename = argumentManager.get("orig");
    if (!strOrigTriFilename.empty()) orig = strOrigTriFilename;

    const std::string strInputFilename = argumentManager.get("input");
    if (!strInputFilename.empty()) input = strInputFilename;

    const std::string strOutputFilename = argumentManager.get("output");
    if (!strOutputFilename.empty()) output = strOutputFilename;

    const std::string strIters = argumentManager.get("iters");
    if (!strIters.empty()) iters = std::stoi(strIters);

    const std::string strCosangle = argumentManager.get("cosangle");
    if (!strCosangle.empty()) cosangle = std::stoi(strCosangle);

    const std::string strPreserveQuality = argumentManager.get("preserveQuality");
    if (!strPreserveQuality.empty()) preserveQuality = strPreserveQuality == "true" ? true : false;

    const std::string strPreserveSharpFeature = argumentManager.get("preserveSharpFeature");
    if (!strPreserveSharpFeature.empty()) preserveSharpFeature = strPreserveSharpFeature == "true" ? true : false;

    const std::string strTreatSharpFeatureAsRegular = argumentManager.get("treatSharpFeatureAsRegular");
    if (!strTreatSharpFeatureAsRegular.empty()) treatSharpFeatureAsRegular = strTreatSharpFeatureAsRegular == "false" ? false : true;

    const std::string strTreatCornerAsRegular = argumentManager.get("treatCornerAsRegular");
    if (!strTreatCornerAsRegular.empty()) treatCornerAsRegular = strTreatCornerAsRegular == "true" ? true : false;

    std::cout << "-----------------------------------\n";
    std::cout << "orig = " << orig << std::endl;
    std::cout << "input = " << input << std::endl;
    std::cout << "output = " << output << std::endl;
    std::cout << "iters = " << iters << std::endl;
    std::cout << "cosangle = " << cosangle << std::endl;
    std::cout << "preserveQuality = " << preserveQuality << std::endl;
    std::cout << "preserveSharpFeature = " << preserveSharpFeature << std::endl;
    std::cout << "treatSharpFeatureAsRegular = " << treatSharpFeatureAsRegular << std::endl;
    std::cout << "treatCornerAsRegular = " << treatCornerAsRegular << std::endl;
    std::cout << "-----------------------------------\n";
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: MeshProject orig=<tri.vtk> input=<quad.vtk> output=<out.vtk> iters=<1> preserveQuality=<false> "
                "preserveSharpFeature=<false> treatSharpFeatureAsRegular=<true> treatCornerAsRegular=<false> cosangle=<0.939692621>\n\n"
                "Info: cos10° = 0.984807753; cos15° = 0.965925826; cos20° = 0.939692621; cos25° = 0.906307787; cos30° = 0.866025404\n";
        return -1;
    }

    std::string orig = "tri.vtk";
    std::string input = "quad.vtk";
    std::string output = "out.vtk";
    size_t iters = 1;
    bool preserveQuality = false;
    bool preserveSharpFeature = false;
    bool treatSharpFeatureAsRegular = true;
    bool treatCornerAsRegular = false;
    double cosangle = 0.939692621;
    ArgumentManager argumentManager(argc, argv);
    GetArguments(argumentManager, orig, input, output, iters, preserveQuality,
            preserveSharpFeature, treatSharpFeatureAsRegular, treatCornerAsRegular, cosangle);

    MeshFileReader origTriReader(orig.c_str());
    Mesh& origTriMesh = (Mesh&)origTriReader.GetMesh();
    origTriMesh.BuildAllConnectivities();
    origTriMesh.ExtractBoundary();
//    origTriMesh.GetNormalOfSurfaceFaces();
//    origTriMesh.GetNormalOfSurfaceVertices();
    origTriMesh.GetAvgEdgeLength();

    MeshFileReader quadReader(input.c_str());
    Mesh& quadMesh = (Mesh&)quadReader.GetMesh();

    quadMesh.BuildAllConnectivities();
    quadMesh.ExtractBoundary();
    quadMesh.SetCosAngleThreshold(cosangle);

    while (iters-- != 0)
    {
        quadMesh.ClearLabelOfSurface();
        quadMesh.LabelSurface();
//        quadMesh.GetNormalOfSurfaceFaces();
//        quadMesh.GetNormalOfSurfaceVertices();

        if (!preserveQuality) {
            quadMesh.SmoothSurface(1, LAPLACE_EDGE, preserveSharpFeature, treatSharpFeatureAsRegular, treatCornerAsRegular);
            quadMesh.ProjectTo(origTriMesh);
        }
        else {
            quadMesh.ProjectSurface(origTriMesh, 1, LAPLACE_EDGE, preserveSharpFeature, treatSharpFeatureAsRegular, treatCornerAsRegular, preserveQuality);
//            quadMesh.SmoothVolume(origTriMesh, 1, LAPLACE_EDGE, preserveQuality);
        }

//        static int iter = 1;
//        std::string filename = std::string("Smooth.") + std::to_string(iter++) + ".vtk";
//        MeshFileWriter writer(quadMesh, filename.c_str());
//        writer.WriteFile();
    }

    MeshFileWriter facesFileWriter(quadMesh, "out_Face.vtk");
    facesFileWriter.WriteFacesVtk();

    MeshFileWriter writer(quadMesh, output.c_str());
    writer.WriteFile();

//    std::vector<FeatureLine> featureLines(origTriMesh.numOfSharpEdges, FeatureLine(origTriMesh));
//    for (size_t i = 0; i < origTriMesh.numOfSharpEdges; i++)
//        featureLines.at(i).Extract(i);
//    quadMesh.ProjectSharpEdgesTo(featureLines);
    return 0;
}



