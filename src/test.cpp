#include <iostream>
#include <odmodel.hpp>

void testSettersGetters(odmodel od1)
{
    std::cout << "\n---------Testing setters and getters---------" << std::endl;
    std::cout << "Screw file name: " << od1.getScrewfilename() << std::endl;
    std::cout << "Db file name: " << od1.getDbfilename() << std::endl;
    std::cout << "Srdb file name: " << od1.getSrdbfilename() << std::endl;
    std::cout << "Interpolation type: " << od1.getInterp_type() << std::endl;
    std::cout << "Rpm_c: " << od1.getRpm_c() << std::endl;
    std::cout << "Flow rate: " << od1.getFlowRate() << std::endl;
    std::cout << "Tin: " << od1.getTin() << std::endl;
    std::cout << "Tsun: " << od1.getTsun() << std::endl;
    std::cout << "Tring1: " << od1.getTring1() << std::endl;
    std::cout << "Tring2: " << od1.getTring2() << std::endl;
    std::cout << "Tring3: " << od1.getTring3() << std::endl;
}

void testReadScrew(odmodel od1)
{
    std::cout << "\n---------Testing read_screw---------" << std::endl;
    od1.read_screw();
    od1.printMixerDescr();
}

void testProcessParameterInnit(odmodel::processParameter process)
{
    std::cout << "\n---------Testing processParameter init---------" << std::endl;
    auto coords = process.getCoord();
    std::cout << "RPM: " << coords[0] << ", MassFlowRate: " << coords[1]
              << ", TSun: " << coords[2] << ", TRing: " << coords[3] << std::endl;
}

void testProcessParameterCp(odmodel::processParameter process)
{
    std::cout << "\n---------Testing processParameter Cp---------" << std::endl;
    double temperature = 350;
    std::cout << "Cp at " << temperature << " K: " << process.calculateCp(temperature) << std::endl;
}

void testInterpolator()
{
    // Creating a simple dataset with 5 data points
    Eigen::MatrixXd data(5, 5);
    data << 1, 2, 3, 4, 5,
        2, 4, 6, 8, 10,
        3, 6, 9, 12, 15,
        4, 8, 12, 16, 20,
        5, 10, 15, 20, 25;

    // Instantiate the interpolator with the dataset
    odmodel::phys_mod::num_mod::interpolator_op interpolator(data);

    // Example point to interpolate
    double point = 6;         // A point we want to interpolate for varIndex 1(2nd column)
    std::size_t varIndex = 1; // Index of the variable (column) to interpolate

    // Perform interpolation using the interp method
    double interpolatedValue = interpolator.interp(varIndex, point);
    std::cout << "Interpolated value at point " << point << " for variable index " << varIndex << " is: " << interpolatedValue << std::endl;

    // Perform nearest neighbor interpolation using interp_safe
    double nearestValue = interpolator.interp_safe(varIndex, point);
    std::cout << "Nearest neighbor value at point " << point << " for variable index " << varIndex << " is: " << nearestValue << std::endl;
}