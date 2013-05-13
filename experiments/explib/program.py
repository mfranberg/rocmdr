import subprocess

from . import power

##
# Wrapper for running Bayesic.
#
class BayesicMethod:
    def __init__(self, name, path):
        self.name = name
        self.path = path

    def run(self, data_prefix, num_tests, output_file, include_covariates = False):
        cmd = [ "python", self.path,
                "-p", data_prefix + ".pair",
                "-n", str( num_tests ),
                data_prefix ]

        if include_covariates:
            cmd.extend( [ "-c", data_prefix + ".cov" ] )

        subprocess.call( cmd, stdout = output_file )

    def compute_power(self, output_path, num_tests):
        return power.compute_from_file( output_path, 2, float.__ge__, 0.95 )

##
# Wrapper for running log-linear method.
#
class LogLinearMethod:
    def __init__(self, name, path):
        self.name = name
        self.path = path

    def run(self, data_prefix, num_tests, output_file, include_covariates = False):
        cmd = [ "Rscript", self.path,
                data_prefix + ".pair",
                data_prefix ]

        subprocess.call( cmd, stdout = output_file )

    def compute_power(self, output_path, num_tests):
        return power.compute_from_file( output_path, 2, float.__le__, 0.05 / num_tests )

##
# Wrapper for running logistic method.
#
class LogisticMethod:
    def __init__(self, name, path):
        self.name = name
        self.path = path

    def run(self, data_prefix, num_tests, output_file, include_covariates = False):
        cmd = [ "Rscript", self.path,
                data_prefix + ".pair",
                data_prefix ]

        if include_covariates:
            cmd.append( data_prefix + ".pair" )
        subprocess.call( cmd, stdout = output_file )

    def compute_power(self, output_path, num_tests):
        return power.compute_from_file( output_path, 2, float.__le__, 0.05 / num_tests )


##
# Returns a list of methods that can run plink files.
#
# @return A list of methods that can run plink files.
#
def get_methods( ):
    return [ BayesicMethod( "Bayesic", "../../py-rocmdr/py-rocmdr.py" ),
             LogLinearMethod( "Log-linear", "../scripts/ll_interaction.r" ),
             LogisticMethod( "Logistic", "../scripts/glm_interaction.r" ) ]

##
# Runs all defined methods on the given plink file.
#
# @param params General experiment parameters.
# @param plink_path Prefix path to the plink, .pair and .cov files.
# @param method_handler Object that handles method output.
#
def run_methods(params, plink_path, method_handler):
    method_list = get_methods( )
    for method in method_list:
        method_output_file = method_handler.get_output_file( method.name )
        method.run( plink_path, params.num_tests, method_output_file )

##
# Calculates power for all the output files found
# in the method handler, and returns a dict mapping method
# names to a tuple that contains power, lower confidence limit,
# and upper confidence limit.
#
# @param param General experiment parameters.
# @param method_handler Object that handles method output.
#
# @return A dict containing power estimates for each method.
#
def calculate_power(params, method_handler):
    method_power = dict( )
    method_list = get_methods( )
    for method in method_list:
        method_output_file = method_handler.get_output_file( method.name )
        power, lower, upper = method.compute_power( method_output_file, params.num_tests )

        method_power[ method.name ] = ( power, lower, upper )

    return method_power
