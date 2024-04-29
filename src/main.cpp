#include "odmodel.hpp"
#include "processParameter.hpp"
#include "phys_mod.hpp"

int main()
{
    odmodel od1;

    od1.setScrewfilename("data/machine.csv");
    od1.setDbfilename("dbfile.txt");
    od1.setSrdbfilename("srdbfile.txt");
    od1.setInterp_type("LS linear"); // Unstructured linear, LS linear, LS bilinear, LS quadratic
    od1.setRpm_c(100.0);
    od1.setFlowRate(200);
    od1.setTin(30);
    od1.setTsun(30);
    od1.setTring1(60);
    od1.setTring2(60);
    od1.setTring3(60);

    // testing ---------

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

    od1.read_screw();
    od1.printMixerDescr();

    odmodel::processParameter process(1200, 800, 300, 250);
    auto coords = process.getCoord();
    std::cout << "RPM: " << coords[0] << ", MassFlowRate: " << coords[1]
              << ", TSun: " << coords[2] << ", TRing: " << coords[3] << std::endl;

    double temperature = 350;
    std::cout << "Cp at " << temperature << " K: " << process.calculateCp(temperature) << std::endl;

    odmodel::phys_mod::num_mod::interpolator_op test1;
    test1.printtest();

    return 0;
}