#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <vector>
#include <algorithm>
#include "halirSpec.hpp"
//#include "specsac.h"

using namespace std;
namespace pt = boost::property_tree;
namespace bf = boost::filesystem;

void HalIRSpec::check_file(string &filename)
{
    bf::path p(filename);
    bf::path tmp = p.parent_path();
    //rootdir = tmp.string();
    //tmp = p.filename();
    //inputfile = tmp.string();
}

HalIRSpec::HalIRSpec()
  : press(-1), temp(-1), res(-1), fov(-1), ext(-1), pathL(-1)
{
    //set_maps();
}

HalIRSpec::HalIRSpec(std::string &infile)
  : press(-1), temp(-1), res(-1), fov(-1), ext(-1), pathL(-1)
{
    /********************
     *     Header       *
     * Set some members  *
     ********************/
    //set_maps();

    pt::ptree root;
    stringstream infil(infile);
    pt::read_json(infil, root);
    // Read Project specific values
    pcomments = root.get<string>("projectDict.pcomments");
    pname = root.get<string>("projectDict.pname");
    pdir = root.get<string>("projectDict.pdir");
    p_db = root.get<string>("projectDict.p_db");
    for (pt::ptree::value_type &val : root.get_child("projectDict.pfiles")) {
      pfiles.push_back(val.second.get_value<string>());
    }

    // Read Environment and sample values
    pathL = root.get<float>("sampleDict.pathL");
    // pathLU = root.get<string>("sampleDict.pathLU");
    temp = root.get<float>("sampleDict.temp");
    // tempU = root.get<string>("sampleDict.tempU");
    press = root.get<float>("sampleDict.press");
    // pressU = root.get<string>("sampleDict.pressU");
    res = root.get<float>("sampleDict.res");
    fov = root.get<float>("sampleDict.fov");
    apod = root.get<string>("sampleDict.apod");
    ftype = root.get<string>("sampleDict.ftype");
    int cc = 0;
    for (pt::ptree::value_type &val : root.get_child("sampleDict.ROI")) {
      ROI[cc] = val.second.get_value<float>();
      cc++;
    }
    for (pt::ptree::value_type &val : root.get_child("sampleDict.comp")) {
      string molec = val.second.get<string>("molec");
      string isotp = val.second.get<string>("isotop");
      float tv = val.second.get<float>("vmr");
      bf::path dir(pdir);
      bf::path file(molec+".hpar");
      bf::path full = dir / file;
      comp.push_back(new Comp(molec, isotp, full.string(), tv));
    }
    for ( auto cmp : comp)
      cmp->parm = hitran.create_molparm(cmp->hpar);
}
HalIRSpec::~HalIRSpec()
{
    //delete conc;
    //delete uconc;
    //delete ug;
    //delete molec;
}
/*
void HalIRSpec::set_maps()
{
    conc_map.emplace("ppm",1E-6);
    conc_map.emplace("ppb",1E-9);
    conc_map.emplace("bar",bar2atm);
    conc_map.emplace("mb",mb2atm);
    conc_map.emplace("pa",pa2atm);
    conc_map.emplace("kpa",kpa2atm);
    conc_map.emplace("hpa",hpa2atm);
    conc_map.emplace("torr",torr2atm);
    conc_map.emplace("atm",1.0);
    press_map.emplace("bar",bar2atm);
    press_map.emplace("mb",mb2atm);
    press_map.emplace("pa",pa2atm);
    press_map.emplace("kpa",kpa2atm);
    press_map.emplace("hpa",hpa2atm);
    press_map.emplace("torr",torr2atm);
    press_map.emplace("atm",1.0);
    path_map.emplace("km",km2cm);
    path_map.emplace("m",m2cm);
    path_map.emplace("dm",dm2cm);
    path_map.emplace("cm",1.0);
}
*/
