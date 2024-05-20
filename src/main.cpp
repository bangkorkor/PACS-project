#include "odmodel.hpp"
#include "processParameter.hpp"
#include "phys_mod.hpp"
#include "phys_mod.hpp"
#include "test.cpp"

int main()
{
    odmodel od1;

    od1.setScrewfilename("data/machine.csv");
    od1.setDbfilename("dbfile.txt");
    od1.setSrdbfilename("srdbfile.txt");
    od1.setInterp_type("LS linear"); // Unstructured linear, LS linear, LS bilinear or LS quadratic
    od1.setRpm_c(100.0);
    od1.setFlowRate(200);
    od1.setTin(30);
    od1.setTsun(30);
    od1.setTring1(60);
    od1.setTring2(60);
    od1.setTring3(60);

    // testing ---------

    testSettersGetters(od1);

    testReadScrew(od1);

    odmodel::processParameter process(1200, 800, 300, 250);

    testProcessParameterInnit(process);
    testProcessParameterCp(process);

    testInterpolator();

    // Example usage
    // Eigen::MatrixXd data(10, 2); // Sample data matrix
    // data << 1, 1, 2, 1, 3, 2, 4, 2, 5, 1, 6, 1, 7, 2, 8, 2, 9, 1, 10, 2;
    // odmodel::phys_mod::num_mod num1("mix", Eigen::VectorXd::LinSpaced(10, 0, 9), nullptr, true);
    // num1.set_db(data);

    return 0;
}