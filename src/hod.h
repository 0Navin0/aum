#ifndef HOD_H
#define HOD_H

#ifndef TINK
#define TINK 0
#endif

#ifndef OFFNEW
#define OFFNEW 0
#endif

#include "cosmology.h"
#include <cmath>
#include "gauleg.h"
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
//Halo occupation distribution function
//Derived from the class cosmology.

struct phi_params
{
    hod *hptr;
    double z;
};

struct proj_params
{
    hod *hptr;
    double *R;
    double *z;
};

struct xi_gd_params
{
    hod *hptr;
    double *r;
    double *z;
};

struct xi_gg_params
{
    hod *hptr;
    double *r;
    double *z;
};

struct hodpars
{
    // HOD parameters
    double Mmin,siglogM,Msat,alpsat,Mcut;

    //
    double fac,csbycdm;

};

double dxi_gg(double,void*);
double dxi_gd(double,void*);
double dwp(double,void*);
double dwp2(double,void*);
double dsigma(double,void*);
double dwp_kaiser(double x, void* params);
double dxiggbarbar(double x, void* params);
double dxiggbar(double x, void* params);

class hod : public cosmology
{
    private:
    /// Numerical interpolation units for D2_gg
    bool bool_init_D2gg;
    gsl_interp_accel *D2gg_acc;
    gsl_spline *D2gg_spline;
    double D2gg_dlow, D2gg_dhigh;
    double D2gg_xlow,D2gg_ylow,D2gg_xhigh,D2gg_yhigh;

    /// Numerical interpolation units for D2_gd
    bool bool_init_D2gd;
    gsl_interp_accel *D2gd_acc;
    gsl_spline *D2gd_spline;
    double D2gd_dlow, D2gd_dhigh;
    double D2gd_xlow,D2gd_ylow,D2gd_xhigh,D2gd_yhigh;
    
    double kPkmax,kPkmax2;

    /// Numerical interpolation units for xi_gg
    bool bool_init_xigg;
    gsl_interp_accel *xigg_acc;
    gsl_spline *xigg_spline;
    double xigg_dlow, xigg_dhigh;
    double xigg_xlow,xigg_ylow,xigg_xhigh,xigg_yhigh;
#ifndef SWIG
    const static double hod_rmin=-4.6;
    const static double hod_rmax= 4.0;
    const static double hod_rmax_u= 2.1;
#else
    const double hod_rmin=-4.6;
    const double hod_rmax= 4.0;
    const double hod_rmax_u= 2.1;
#endif

    /// Numerical interpolation units for xi_gd
    bool bool_init_xigd;
    gsl_interp_accel *xigd_acc;
    gsl_spline *xigd_spline;
    double xigd_dlow, xigd_dhigh;
    double xigd_xlow,xigd_ylow,xigd_xhigh,xigd_yhigh;

    /// Numerical interpolation units for SD
    bool bool_init_sd;
    gsl_interp_accel *sd_acc;
    gsl_spline *sd_spline;

#if TINK==2
    /// Numerical interpolation units for SD
    bool bool_init_nc;
    gsl_interp_accel *nc_acc;
    gsl_spline *nc_spline;

    double nc_mmin;
    double nc_mmax;

    /// Numerical interpolation units for SD
    bool bool_init_ns;
    gsl_interp_accel *ns_acc;
    gsl_spline *ns_spline;

    double ns_mmin;
    double ns_mmax;

#endif

    /// Halo exclusion
    bool halo_exc;

    /// Mis-centering kernel for central galaxies
    double off_rbyrs;
    double fcen_off;

    /// Incompleteness
    double inc_alp;
    double inc_xM;

    /// Kaiser factor
    double fKaiser;
    
    /// Kaiser correction routines
    double xigg_barbar(double r,double z);
    void init_xigg_barbar(double z);
    double xigg_barbar_num(double r,double z);
    double xigg_bar(double r,double z);
    void init_xigg_bar(double z);
    double xigg_bar_num(double r,double z);
    /// Numerical interpolation units
#ifndef SWIG
    const static int Nxibar=200;
#else
    const int Nxibar=200;
#endif
    bool bool_init_xigg_bar;
    gsl_interp_accel *xigg_bar_acc;
    gsl_spline *xigg_bar_spline;
    bool bool_init_xigg_barbar;
    gsl_interp_accel *xigg_barbar_acc;
    gsl_spline *xigg_barbar_spline;
    
    public:
    int whichopt;
    hodpars hodp;
    hod(cosmo, hodpars);
    hod();

    double ncen(double);           // <Ncen>(M)
    double nsat(double);           // <Nsat>(M)
    double ncenz(double);           // <Ncen>(z)
    double nsatz(double);           // <Nsat>(z)
    double avmass_tot(double);
    double avmass_cen(double);
    double galaxy_bias(double);
#ifndef SWIG
	const static int Np_nsat=100;
#else
	const int Np_nsat=100;
#endif
	double xp[Np_nsat],wp[Np_nsat];
    double gets8();
    double geth();
    double getOmb();
    double getOmk();
    double set_cfactor(double);
    void changepar(cosmo &, hodpars &, int,double, double &);
    void changepar(cosmo &, hodpars &, int,double);
    void getnewpar(cosmo &, hodpars &, cosmo, hodpars, gsl_rng *& );
    void getnewpar(cosmo &, hodpars &, gsl_rng *&, double * , int *, int);

    /// Power spectra
    double Pk_gg_gd(double);
    double Pk_gg_gd_he(double);

    /// Power spectra interpolation
    double D2gg_num(double,double);
    double D2gd_num(double,double);

    /// Correlation functions
    double xi_gg_gd(double);
    double xi_gg(double);
    double xi_gd(double);

    /// Numerical interpolation of correlation functions
    double xigg_num(double,double);
    double xigd_num(double,double);

    /// The projection integrals
    double Wp_ESD(double,int,int,double[],double[],double[],double[],int,double,bool reset=true);
    double Wp(double,int,double[],double[],double,bool reset=true);
    double Wp_Kaiser(double,int,double[],double[],double,bool reset=true);
    double ESD(double, int, double[], double[],int,bool reset=true);
    double Sigma(double, int, double[], double[],bool reset=true, double xfgm_m0=-99.0, double xfgm_slp=-99.0,double pimax=-80.0);

    /// The halo model power spectrum
    double pspec_halomodel(double z);
    
    double scale_dep_bias_crossr(double,int,double[],double[],double[],bool);

    void resetz(double);

    void hod_free();

    void print(cosmo, hodpars);
    void print(FILE*&,cosmo, hodpars, double,double);
    void print(FILE*&,cosmo, hodpars);
    void print();

    void sethalo_exc(bool);
    void set_cen_offset_params(double,double);
    void set_inc_params(double,double);

    /// Friends
    friend double dxi_gg(double,void*);
    friend double dxi_gd(double,void*);
    friend double dwp(double,void*);
    friend double dwp2(double,void*);
    friend double dsigma(double,void*);
    friend double dwp_kaiser(double x, void* params);
    friend double dxiggbarbar(double x, void* params);
    friend double dxiggbar(double x, void* params);

    // Renew parameters
    void hod_renew(cosmo p, hodpars h);

#if TINK==2
    void init_Nc_spl(double xx[],double yy[],int Ncspl);
    void init_Ns_spl(double xx[],double yy[],int Nsspl);
#endif

};

#endif
