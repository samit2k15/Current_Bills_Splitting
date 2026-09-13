/*
 * Electricity bill calculator
 *
 * Portable C11 implementation of the original Turbo C application.
 * Build with:
 *     cc -std=c11 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Werror -x c ELECTRIC.C -o electricity_bills
 *
 * The program supports:
 *   1. Allocating a known electricity amount across multiple meters.
 *   2. Calculating multiple meters from a supplied per-unit rate.
 *   3. Calculating one meter using the historical Medak tariff table.
 */

#include <errno.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_METERS 100U
#define OUTPUT_FILE "current.txt"
#define INPUT_SIZE 256U
#define EPSILON 0.000001

typedef struct {
    char meter_number[64];
    long present_reading;
    long previous_reading;
    double units;
    double energy_cost;
    double extra_charge;
    double total_cost;
} MeterBill;

typedef struct {
    double rate;
    const char *label;
} Tariff;

static const Tariff TARIFFS[] = {
    {1.45, "up to 50 units"},
    {2.60, "51 to 100 units"},
    {4.30, "101 to 200 units"},
    {7.20, "201 to 300 units"},
    {8.50, "301 to 400 units"},
    {9.00, "401 to 800 units"},
    {9.50, "above 800 units"}
};

static void discard_rest_of_line(void)
{
    int character;
    while ((character = getchar()) != '\n' && character != EOF) {
        /* Discard remaining input. */
    }
}

static bool read_line(const char *prompt, char *buffer, size_t size)
{
    if (prompt != NULL) {
        fputs(prompt, stdout);
        fflush(stdout);
    }

    if (fgets(buffer, (int)size, stdin) == NULL) {
        return false;
    }

    if (strchr(buffer, '\n') == NULL) {
        discard_rest_of_line();
    }

    buffer[strcspn(buffer, "\r\n")] = '\0';
    return true;
}

static bool parse_long(const char *text, long *value)
{
    char *end = NULL;
    long parsed;

    errno = 0;
    parsed = strtol(text, &end, 10);
    if (text == end || errno == ERANGE || *end != '\0') {
        return false;
    }

    *value = parsed;
    return true;
}

static bool parse_double(const char *text, double *value)
{
    char *end = NULL;
    double parsed;

    errno = 0;
    parsed = strtod(text, &end);
    if (text == end || errno == ERANGE || *end != '\0') {
        return false;
    }

    *value = parsed;
    return isfinite(parsed);
}

static bool read_nonnegative_double(const char *prompt, double *value)
{
    char input[INPUT_SIZE];
    double parsed;

    for (;;) {
        if (!read_line(prompt, input, sizeof input)) {
            return false;
        }
        if (parse_double(input, &parsed) && parsed >= 0.0) {
            *value = parsed;
            return true;
        }
        puts("Please enter a valid non-negative number.");
    }
}

static bool read_positive_double(const char *prompt, double *value)
{
    char input[INPUT_SIZE];
    double parsed;

    for (;;) {
        if (!read_line(prompt, input, sizeof input)) {
            return false;
        }
        if (parse_double(input, &parsed) && parsed > 0.0) {
            *value = parsed;
            return true;
        }
        puts("Please enter a number greater than zero.");
    }
}

static bool read_meter_count(size_t *count)
{
    char input[INPUT_SIZE];
    long parsed;

    for (;;) {
        if (!read_line("How many meters do you have (1-100)? ", input, sizeof input)) {
            return false;
        }
        if (parse_long(input, &parsed) && parsed >= 1L && parsed <= (long)MAX_METERS) {
            *count = (size_t)parsed;
            return true;
        }
        printf("Please enter an integer from 1 to %u.\n", MAX_METERS);
    }
}

static bool read_meter_number(size_t index, char *meter_number, size_t size)
{
    char prompt[INPUT_SIZE];

    (void)snprintf(prompt, sizeof prompt, "Meter number for meter %zu: ", index);
    for (;;) {
        if (!read_line(prompt, meter_number, size)) {
            return false;
        }
        if (meter_number[0] != '\0') {
            return true;
        }
        puts("Meter number cannot be empty.");
    }
}

static bool read_meter_readings(size_t index, long *present, long *previous)
{
    char input[INPUT_SIZE];
    char prompt[INPUT_SIZE];
    long present_value;
    long previous_value;

    (void)snprintf(prompt, sizeof prompt, "Present reading for meter %zu: ", index);
    for (;;) {
        if (!read_line(prompt, input, sizeof input)) {
            return false;
        }
        if (parse_long(input, &present_value) && present_value >= 0L) {
            break;
        }
        puts("Please enter a non-negative integer reading.");
    }

    (void)snprintf(prompt, sizeof prompt, "Previous reading for meter %zu: ", index);
    for (;;) {
        if (!read_line(prompt, input, sizeof input)) {
            return false;
        }
        if (parse_long(input, &previous_value) && previous_value >= 0L && previous_value <= present_value) {
            break;
        }
        puts("Please enter a non-negative reading not greater than the present reading.");
    }

    *present = present_value;
    *previous = previous_value;
    return true;
}

static double tariff_rate_for_units(double units)
{
    if (units <= 50.0) {
        return TARIFFS[0].rate;
    }
    if (units <= 100.0) {
        return TARIFFS[1].rate;
    }
    if (units <= 200.0) {
        return TARIFFS[2].rate;
    }
    if (units <= 300.0) {
        return TARIFFS[3].rate;
    }
    if (units <= 400.0) {
        return TARIFFS[4].rate;
    }
    if (units <= 800.0) {
        return TARIFFS[5].rate;
    }
    return TARIFFS[6].rate;
}

static const char *tariff_label_for_units(double units)
{
    if (units <= 50.0) {
        return TARIFFS[0].label;
    }
    if (units <= 100.0) {
        return TARIFFS[1].label;
    }
    if (units <= 200.0) {
        return TARIFFS[2].label;
    }
    if (units <= 300.0) {
        return TARIFFS[3].label;
    }
    if (units <= 400.0) {
        return TARIFFS[4].label;
    }
    if (units <= 800.0) {
        return TARIFFS[5].label;
    }
    return TARIFFS[6].label;
}

static bool collect_meters(MeterBill *meters, size_t count, double *total_units)
{
    size_t index;
    double units = 0.0;

    for (index = 0U; index < count; ++index) {
        if (!read_meter_number(index + 1U, meters[index].meter_number, sizeof meters[index].meter_number) ||
            !read_meter_readings(index + 1U, &meters[index].present_reading, &meters[index].previous_reading)) {
            return false;
        }
        meters[index].units = (double)(meters[index].present_reading - meters[index].previous_reading);
        units += meters[index].units;
    }

    *total_units = units;
    return true;
}

static bool open_output_file(FILE **file)
{
    *file = fopen(OUTPUT_FILE, "w");
    if (*file == NULL) {
        fprintf(stderr, "Warning: could not write %s: %s\n", OUTPUT_FILE, strerror(errno));
        return false;
    }
    return true;
}

static void print_bill(FILE *output, const MeterBill *meters, size_t count,
                       double total_units, double energy_total, double extra_total,
                       const char *calculation_description)
{
    size_t index;
    double grand_total = energy_total + extra_total;

    printf("\n%s\n", calculation_description);
    puts("-------------------------------------------------------------------------------");
    printf("%-4s %-20s %10s %14s %14s %14s\n", "No.", "Meter", "Units", "Energy", "Extra", "Total");
    puts("-------------------------------------------------------------------------------");

    if (output != NULL) {
        fprintf(output, "%s\n", calculation_description);
        fprintf(output, "-------------------------------------------------------------------------------\n");
        fprintf(output, "%-4s %-20s %10s %14s %14s %14s\n", "No.", "Meter", "Units", "Energy", "Extra", "Total");
        fprintf(output, "-------------------------------------------------------------------------------\n");
    }

    for (index = 0U; index < count; ++index) {
        printf("%-4zu %-20s %10.2f %14.2f %14.2f %14.2f\n",
               index + 1U, meters[index].meter_number, meters[index].units,
               meters[index].energy_cost, meters[index].extra_charge, meters[index].total_cost);
        if (output != NULL) {
            fprintf(output, "%-4zu %-20s %10.2f %14.2f %14.2f %14.2f\n",
                    index + 1U, meters[index].meter_number, meters[index].units,
                    meters[index].energy_cost, meters[index].extra_charge, meters[index].total_cost);
        }
    }

    puts("-------------------------------------------------------------------------------");
    printf("Total units: %.2f | Energy: %.2f | Extra charges: %.2f | Grand total: %.2f\n",
           total_units, energy_total, extra_total, grand_total);
    if (output != NULL) {
        fprintf(output, "-------------------------------------------------------------------------------\n");
        fprintf(output, "Total units: %.2f | Energy: %.2f | Extra charges: %.2f | Grand total: %.2f\n",
                total_units, energy_total, extra_total, grand_total);
    }
}

static void calculate_from_amount(void)
{
    MeterBill meters[MAX_METERS] = {0};
    size_t count;
    size_t index;
    double amount;
    double extra_total;
    double total_units;
    double energy_total = 0.0;
    FILE *output = NULL;

    if (!read_meter_count(&count) || !read_positive_double("Total electricity amount (excluding extra charges): ", &amount) ||
        !read_nonnegative_double("Total extra charges: ", &extra_total) || !collect_meters(meters, count, &total_units)) {
        return;
    }
    if (total_units <= 0.0) {
        puts("The total consumed units must be greater than zero.");
        return;
    }

    for (index = 0U; index < count; ++index) {
        meters[index].energy_cost = amount * meters[index].units / total_units;
        meters[index].extra_charge = extra_total / (double)count;
        meters[index].total_cost = meters[index].energy_cost + meters[index].extra_charge;
        energy_total += meters[index].energy_cost;
    }

    (void)open_output_file(&output);
    print_bill(output, meters, count, total_units, energy_total, extra_total,
               "Bill allocation from supplied electricity amount");
    if (output != NULL) {
        fclose(output);
        printf("Saved a copy to %s.\n", OUTPUT_FILE);
    }
}

static void calculate_from_rate(void)
{
    MeterBill meters[MAX_METERS] = {0};
    size_t count;
    size_t index;
    double rate;
    double extra_total;
    double total_units;
    double energy_total = 0.0;
    FILE *output = NULL;

    if (!read_meter_count(&count) || !read_positive_double("Per-unit cost: ", &rate) ||
        !read_nonnegative_double("Total extra charges: ", &extra_total) || !collect_meters(meters, count, &total_units)) {
        return;
    }

    for (index = 0U; index < count; ++index) {
        meters[index].energy_cost = rate * meters[index].units;
        meters[index].extra_charge = extra_total / (double)count;
        meters[index].total_cost = meters[index].energy_cost + meters[index].extra_charge;
        energy_total += meters[index].energy_cost;
    }

    (void)open_output_file(&output);
    print_bill(output, meters, count, total_units, energy_total, extra_total,
               "Bill calculation from supplied per-unit cost");
    if (output != NULL) {
        fclose(output);
        printf("Saved a copy to %s.\n", OUTPUT_FILE);
    }
}

static void calculate_default_tariff(void)
{
    MeterBill meter = {0};
    double extra_charge;
    double rate;
    FILE *output = NULL;

    puts("\nHistorical Medak household tariff (flat rate by consumption band)");
    if (!read_meter_number(1U, meter.meter_number, sizeof meter.meter_number) ||
        !read_meter_readings(1U, &meter.present_reading, &meter.previous_reading) ||
        !read_nonnegative_double("Extra charges: ", &extra_charge)) {
        return;
    }

    meter.units = (double)(meter.present_reading - meter.previous_reading);
    if (meter.units <= 0.0) {
        puts("Consumed units must be greater than zero.");
        return;
    }

    rate = tariff_rate_for_units(meter.units);
    meter.energy_cost = meter.units * rate;
    meter.extra_charge = extra_charge;
    meter.total_cost = meter.energy_cost + meter.extra_charge;

    printf("\nTariff band: %s at %.2f per unit\n", tariff_label_for_units(meter.units), rate);
    (void)open_output_file(&output);
    print_bill(output, &meter, 1U, meter.units, meter.energy_cost, meter.extra_charge,
               "Single-meter bill using default tariff");
    if (output != NULL) {
        fclose(output);
        printf("Saved a copy to %s.\n", OUTPUT_FILE);
    }
}

static bool read_menu_choice(int *choice)
{
    char input[INPUT_SIZE];
    long parsed;

    if (!read_line("Select an option: ", input, sizeof input)) {
        return false;
    }
    if (!parse_long(input, &parsed) || parsed < 1L || parsed > 4L) {
        puts("Please select 1, 2, 3, or 4.");
        return true;
    }
    *choice = (int)parsed;
    return true;
}

int main(void)
{
    int choice = 0;

    puts("Electricity Bill Calculator");
    puts("===========================");
    puts("Rates are preserved from the original Medak-district program.");

    while (choice != 4) {
        puts("\n1. Allocate a known electricity amount across multiple meters");
        puts("2. Calculate multiple meters from a per-unit cost");
        puts("3. Calculate one meter using the default tariff");
        puts("4. Exit");

        if (!read_menu_choice(&choice)) {
            putchar('\n');
            break;
        }

        switch (choice) {
        case 1:
            calculate_from_amount();
            break;
        case 2:
            calculate_from_rate();
            break;
        case 3:
            calculate_default_tariff();
            break;
        case 4:
            puts("Program ended.");
            break;
        default:
            /* read_menu_choice handles invalid values. */
            break;
        }
    }

    return EXIT_SUCCESS;
}
