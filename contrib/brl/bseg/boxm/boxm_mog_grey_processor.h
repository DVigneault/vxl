// This is vxl/contrib/brl/bbas/boxm/boxm_mog_grey_processor.h
#ifndef boxm_mog_grey_processor_h_
#define boxm_mog_grey_processor_h_
//:
// \file
// \brief A class for a grey-scale-mixture-of-gaussian processor
//
// \author Gamze Tunali
// \date   04/03/2009
// \verbatim
//  Modifications
//  
// \endverbatim


#include <bsta/algo/bsta_adaptive_updater.h>
#include <bsta/bsta_distribution.h>
#include <bsta/bsta_gauss_f1.h>
#include <bsta/bsta_attributes.h>
#include <bsta/bsta_mixture_fixed.h>
#include <bsta/bsta_gaussian_indep.h>


#include "boxm_apm_traits.h"


class  boxm_mog_grey_processor
{
protected:
  // convienance typedefs
  typedef boxm_apm_traits<BOXM_APM_MOG_GREY>::apm_datatype apm_datatype;
  typedef boxm_apm_traits<BOXM_APM_MOG_GREY>::obs_datatype obs_datatype;
  typedef boxm_apm_traits<BOXM_APM_MOG_GREY>::obs_mathtype obs_mathtype;

public:
  static boxm_apm_traits<BOXM_APM_MOG_GREY>::obs_datatype 
	  expected_color(boxm_apm_traits<BOXM_APM_MOG_GREY>::apm_datatype const& appear);


private:


};

#endif // boxm_mog_grey_processor_h_
