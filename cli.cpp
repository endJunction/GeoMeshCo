
#include "cli.h"

#include <iostream>
#include <string>
#include <tclap/CmdLine.h>

const std::string message("MeshScape.   \
Copyright (C) 2012 Dmitrij Yu. Naumov.  This program comes \
with ABSOLUTELY NO WARRANTY;  This is free software, and you are \
welcome to redistribute it under certain conditions;  For details \
see the LICENSE file.");

const char delimiter(' ');
const std::string version("0.1");

TCLAP::ValueArg<std::string>*
construct_input_arg(const std::string& d, const bool r = true)
{
    return new TCLAP::ValueArg<std::string>(
        "i", "input", d, r, std::string(), "FILE");
}

TCLAP::ValueArg<std::string>*
construct_output_arg(const std::string& d, const bool r = true)
{
    return new TCLAP::ValueArg<std::string>(
        "o", "output", d, r, std::string(), "FILE");
}


//// CGAL mesh constraint arguments. ////////////////////////////////
struct CGAL_mesh_constraint_args {
    TCLAP::ValueArg<double> angular_bound;
    TCLAP::ValueArg<double> facet_size;
    TCLAP::ValueArg<double> distance_bound;

    CGAL_mesh_constraint_args(TCLAP::CmdLineInterface& cmd)
        : angular_bound(
            "a",
            "angular_bound",
            "angular bound in the Surface mesh criteria. (30)",
            false,
            30,
            "FLOAT")
        , facet_size(
            "f",
            "facet_size",
            "Facet criteria facet's size. (0.3)",
            false,
            0.3,
            "FLOAT")
        , distance_bound(
            "r",
            "approximation",
            "Facet criteria approximation. (0.1)",
            false,
            0.1,
            "FLOAT")
    {
        cmd.add(facet_size);
        cmd.add(angular_bound);
        cmd.add(distance_bound);
    }
};

CLI::CLI(int argc, char** argv)
    : z_scale(1),
    angular_bound(30),
    approximation(0.1),
    facet_size(0.3),
    input(""),
    output("")
{
    try {
        TCLAP::CmdLine cmd(message, delimiter, version);

        TCLAP::ValueArg<double> z_scale_arg(
            "z",
            "z_scale",
            "Vertical scale factor. (1.0)",
            false,
            1.,
            "FLOAT");
        cmd.add(z_scale_arg);

        CGAL_mesh_constraint_args constraint_args(cmd);

        TCLAP::ValueArg<std::string>* output_arg = construct_output_arg(
            "Write meshes to this file. No output if the filename is not given.",
            false);
        cmd.add(output_arg);

        TCLAP::ValueArg<std::string>* input_arg = construct_input_arg(
            "Input 2D grayscale image.");
        cmd.add(input_arg);


        cmd.parse(argc, argv);

        input = input_arg->getValue();
        output = output_arg->getValue();

        angular_bound = constraint_args.angular_bound.getValue();
        approximation = constraint_args.distance_bound.getValue();
        facet_size = constraint_args.facet_size.getValue();

        z_scale = z_scale_arg.getValue();

    } catch (TCLAP::ArgException &e) {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
    }
}
