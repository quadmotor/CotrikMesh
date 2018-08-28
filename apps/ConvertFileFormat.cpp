/*
 * ConvertFileFormat.cpp
 *
 *  Created on: Dec 29, 2017
 *      Author: cotrik
 */

#include "MeshFileReader.h"
#include "MeshFileWriter.h"
#include "SmoothAlgorithm.h"
#include <iostream>
#include "ArgumentManager.h"
int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: ConvertFileFormat input_file output_file\n";
        return -1;
    }
    ArgumentManager argumentManager(argc, argv);
    MeshFileReader reader(argv[1]);
    Mesh& mesh = (Mesh&)reader.GetMesh();
    MeshFileWriter writer(mesh, argv[2]);
    writer.WriteFile();
    return 0;
}