#include "main.h"
#include "radial_distribution_analyzer.h"
#include "distribution_factory.h"
#include "utils.h"
#include "radial_distribution.h"
#include "reproducer.h"
#include "main_window.h"

#include <iostream>
#include <string>
#include <QApplication>

#define CREATOR_BIN "RadialDistributionFileCreator"
#define REPRODUCER_BIN "RadialDistributionReproducer"
#define ANALYSER_BIN "RadialDistributionFileAnalyzer"
#define GUI_BIN "RadialDistributionToolGUI"


static void print_file_production_usage()
{
    std::cerr << "Usage: " << CREATOR_BIN<< " <option(s)> OUTPUT_FILE\n"
        << "Options:\n"
        << "\t-h,--help\tShow this help message\n"
        << "\t-width\t Area width\n"
        << "\t-height\t Area height\n"
        << "\t-r-min\t R min\n"
        << "\t-r-max\t R max\n"
        << "\t-r-diff\t R diff\n"
        << "\t-random-distribution\tproduce a file with randomly distributed points\n"
            << "\t\t-n-points\t number of random points to produce\n"
        << "\t-seeded-distribution\tproduce a file with seeded distributed points\n"
            << "\t\t-n-seeds\t Number of starting seeds\n"
            << "\t\t-max-seeding-distance\t Maximum distance for a seed can seed to\n"
            << "\t\t-n-seeding-iterations\t Number of seeding iterations\n"
        << "\t-grid-distribution\tproduce a file with randomly distributed points\n"
            << "\t\t-grid-separation\tseparation between two points on the grid\n"
        << "\t-v\tverbose\n"

        << std::endl;
}

static void print_distribution_reproducer_usage()
{
    std::cerr << "Usage: " << REPRODUCER_BIN<< " <option(s)> INPUT_RADIAL_DISTRIBUTION_FILE OUTPUT_IMG_FILE\n"
        << "Options:\n"
        << "\t-h,--help\t\tShow this help message\n"
        << "\t-width\t Area width\n"
        << "\t-height\t Area height\n"
        << "\t-n-iterations\t Number of iterations\n"
        << "\t-v\tverbose\n"
        << std::endl;
}

static void print_radial_distribution_file_analyzer_usage()
{
    std::cerr << "Usage: " << ANALYSER_BIN<< " <option(s)> INPUT_RADIAL_DISTRIBUTION_FILE OUTPUT_CSV_FILE\n"
        << "Options:\n"
        << "\t-h,--help\t\tShow this help message\n"
        << std::endl;
}


#define TMP_IMAGE_FILE "/home/harry/radial_distributions/tmp.jpg"
int main(int argc, char *argv[])
{
    std::cout << "Starting GUI..." << std::endl;
    QApplication app(argc, argv);
    MainWindow w;
    w.resize(w.sizeHint());
    w.showMaximized();

    return app.exec();
}
////    std::string bin(argv[0]);
////    if(bin.find(CREATOR_BIN) != std::string::npos)
////    {
////        if(argc < 8)
////        {
////            print_file_production_usage();
////            return 1;
////        }

////        int height(-1), width(-1), r_min(-1), r_max(-1), r_diff(-1), grid_separation(-1), n_points(-1), n_seeds(-1), max_seeding_dist(-1), n_seeding_iterations(-1);
////        std::string distribution(""), output_file("");
////        bool verbose(false);
////        for(int i(1); i < argc; i++)
////        {
////            std::string arg(argv[i]);
////            if(arg == "-h" || arg == "--help")
////            {
////                print_file_production_usage();
////                return 0;
////            }

////            if(arg == "-width")
////            {
////                if(i+1 > argc)
////                {
////                    print_file_production_usage();
////                    return 1;
////                }
////                width = std::stoi(argv[++i]);
////            }
////            else if(arg == "-height")
////            {
////                if(i+1 > argc)
////                {
////                    print_file_production_usage();
////                    return 1;
////                }
////                height = std::stoi(argv[++i]);
////            }
////            else if(arg == "-r-min")
////            {
////                if(i+1 > argc)
////                {
////                    print_file_production_usage();
////                    return 1;
////                }
////                r_min = std::stoi(argv[++i]);
////            }
////            else if(arg == "-r-max")
////            {
////                if(i+1 > argc)
////                {
////                    print_file_production_usage();
////                    return 1;
////                }
////                r_max = std::stoi(argv[++i]);
////            }
////            else if(arg == "-r-diff")
////            {
////                if(i+1 > argc)
////                {
////                    print_file_production_usage();
////                    return 1;
////                }
////                r_diff = std::stoi(argv[++i]);
////            }
////            else if(arg == "-v")
////            {
////                verbose = true;
////            }
////            // RANDOM DISTRIBUTION
////            else if(arg == "-random-distribution")
////            {
////                distribution = "random-distribution";
////            }
////            else if(arg == "-n-points")
////            {
////                if(i+1 > argc)
////                {
////                    print_file_production_usage();
////                    return 1;
////                }
////                n_points = std::stoi(argv[++i]);
////            }
////            // GRIDDED DISTRIBUTION
////            else if(arg == "-grid-distribution")
////            {
////                distribution = "grid-distribution";
////            }

////            else if(arg == "-grid-separation")
////            {
////                if(i+1 > argc)
////                {
////                    print_file_production_usage();
////                    return 1;
////                }
////                grid_separation = std::stoi(argv[++i]);
////            }
////            // SEEDED ITERATION
////            else if(arg == "-seeded-distribution")
////            {
////                distribution = "seeded-distribution";
////            }
////            else if(arg == "-n-seeds")
////            {
////                if(i+1 > argc)
////                {
////                    print_file_production_usage();
////                    return 1;
////                }
////                n_seeds = std::stoi(argv[++i]);
////            }
////            else if(arg == "-max-seeding-distance")
////            {
////                if(i+1 > argc)
////                {
////                    print_file_production_usage();
////                    return 1;
////                }
////                max_seeding_dist = std::stoi(argv[++i]);
////            }
////            else if(arg == "-n-seeding-iterations")
////            {
////                if(i+1 > argc)
////                {
////                    print_file_production_usage();
////                    return 1;
////                }
////                n_seeding_iterations = std::stoi(argv[++i]);
////            }
////            else // Must be the file
////            {
////                output_file = argv[i];
////            }
////        }
////        // Check all necessary parameters are set
////        if(height == -1 || width == -1 || r_min == -1 || r_max == -1 || r_diff == -1 || distribution == "" || output_file == "" ||
////                (distribution == "random-distribution" && n_points == -1) ||
////                (distribution == "grid-distribution" && grid_separation == -1) ||
////                (distribution == "seeded-distribution" && (n_seeds == -1 || max_seeding_dist == -1 || n_seeding_iterations == -1)) )
////        {
////            print_file_production_usage();
////            return 1;
////        }
////        std::vector<QPoint> points;
////        DistributionFactory distribution_factory;
////        if(distribution == "random-distribution")
////            points = distribution_factory.generateRandomDistribution(n_points, width, height);
////        else if(distribution == "grid-distribution")
////            points = distribution_factory.generateGriddedDistribution(grid_separation, width, height);
////        else if(distribution == "seeded-distribution")
////            points = distribution_factory.generateSeededDistribution(n_seeds, n_seeding_iterations, max_seeding_dist, width,height);

////        if(verbose)
////        {
////            FileUtils::printPointsToImg(TMP_IMAGE_FILE, points, width, height);
////            std::cout << "Generated distirbution points printed to: " << TMP_IMAGE_FILE << std::endl;
////        }

////        RadialDistributionAnalyzer analyzer(r_min, r_max, r_diff);
////        std::cout << "Generating radial distribution file..." << std::flush;
////        RadialDistribution radial_distribution(analyzer.getRadialDistribution(points,points,width,height,0,1));
////        std::cout << "OK" << std::endl;

////        radial_distribution.write(output_file);
////        std::cout << "File saved to: " << output_file << std::endl;
////    }
////    else if(bin.find(REPRODUCER_BIN) != std::string::npos) // Reproduction
////    {
////        if(argc < 8)
////        {
////            print_distribution_reproducer_usage();
////            return 1;
////        }

////        int height(-1), width(-1), n_iterations(-1);
////        std::string input_file(""), output_file("");
////        bool verbose(false);
////        for(int i(1); i < argc; i++)
////        {
////            std::string arg(argv[i]);
////            if(arg == "-h" || arg == "--help")
////            {
////                print_distribution_reproducer_usage();
////                return 0;
////            }

////            if(arg == "-width")
////            {
////                if(i+1 > argc)
////                {
////                    print_distribution_reproducer_usage();
////                    return 1;
////                }
////                width = std::stoi(argv[++i]);
////            }
////            else if(arg == "-height")
////            {
////                if(i+1 > argc)
////                {
////                    print_distribution_reproducer_usage();
////                    return 1;
////                }
////                height = std::stoi(argv[++i]);
////            }
////            else if(arg == "-n-iterations")
////            {
////                if(i+1 > argc)
////                {
////                    print_distribution_reproducer_usage();
////                    return 1;
////                }
////                n_iterations = std::stoi(argv[++i]);
////            }
////            else if(arg == "-v")
////            {
////                verbose = true;
////            }
////            else if(input_file == "")
////            {
////                input_file = arg;
////            }
////            else // must be the output file
////            {
////                output_file = arg;
////            }
////        }
////        if(height == -1 || width == -1 || n_iterations == -1 || input_file == "" || output_file == "")
////        {
////            print_distribution_reproducer_usage();
////            return 1;
////        }
////        RadialDistributionProducer producer(input_file);
////        std::vector<QPoint> reproduced_points(producer.generatePoints(width, height, n_iterations));
////        FileUtils::printPointsToImg(output_file, reproduced_points, width, height);
////        std::cout << "Reproduced file saved to: " << output_file << std::endl;
////    }
////    else if(bin.find(ANALYSER_BIN) != std::string::npos) // Analysis
////    {
////        if(argc < 2)
////        {
////            print_radial_distribution_file_analyzer_usage();
////            return 1;
////        }

////        std::string radial_distribution_file(""), output_csv_file("");
////        for(int i(1); i < argc; i++)
////        {
////            std::string arg(argv[i]);
////            if(arg == "-h" || arg == "--help")
////            {
////                print_radial_distribution_file_analyzer_usage();
////                return 0;
////            }
////            else if(radial_distribution_file == "")
////            {
////                radial_distribution_file = arg;
////            }
////            else
////            {
////                output_csv_file = arg;
////            }
////        }
////        if(radial_distribution_file == "" || output_csv_file == "")
////        {
////            print_radial_distribution_file_analyzer_usage();
////            return 1;
////        }

////        RadialDistribution radial_distribution(radial_distribution_file);
////        radial_distribution.writeToCSV(output_csv_file);

////        std::cout << "Distribution written to: " << output_csv_file << std::endl;
////    }
////    else if(bin.find(GUI_BIN) != std::string::npos) // GUI
////    {
////        std::cout << "Starting GUI..." << std::endl;
////        QApplication app(argc, argv);
////        MainWindow w;
////        w.resize(w.sizeHint());
////        w.showMaximized();

////        return app.exec();
////    }
////    else // Should never come here
////    {
////        std::cerr << "Unknown command!" << std::endl;
////    }


////    return 0;
//}
