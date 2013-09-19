// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT



#ifndef DEISOTOP_H
#define DEISOTOP_H

class Deisotop{

/*
 * sort peaks by mass.
 * start with peak of smallest mass find all peaks that fullfill a few condistions, i.e.
 * mass dinstance less than 1Da + error
 * rt deviation less than 4 seconds
 * create a list of pairs which fullfill condition
 * i.e.
 *[1,4] - > dist mass, dist rt, ratio intensity.
 *[1,8]
 *[1,13]
 */
};

#endif // DEISOTOP_H
