#ifndef AI_MATRIX_LOCATOR_H
#define AI_MATRIX_LOCATOR_H

struct vector;
struct matrix;
struct AI;

void print_v(struct vector*);
void print_m(struct matrix*);
void fill_random_v(struct vector*);
void fill_random_m(struct matrix*);
struct vector* make_vector(unsigned int);
struct matrix* make_matrix(unsigned int, unsigned int);
struct vector* make_vector_from_txt(char*);
void free_vector(struct vector*);
void free_matrix(struct matrix*);
struct vector* multiply(struct matrix*, struct vector*);
struct vector* multiply_add_bias(struct matrix*, struct vector*);
struct vector* multiply_elements_v_v(struct vector*, struct vector*);
struct matrix* multiply_v_vt(struct vector*, struct vector*, struct AI*);
void multiply_scalar_v(struct vector*, double);
struct matrix* divide_scalar_m(struct matrix*, double);
void matrix_fill(struct matrix* mat, double);
void matrix_add(struct matrix*, struct matrix*);
struct matrix* transpose_m(struct matrix*);
struct vector* vector_subtract(struct vector*, struct vector*);
void activation_function(struct vector*);
struct vector* derivative_activation_function(struct vector*);
struct AI* create_AI(int*, unsigned short, double);
void free_AI(struct AI*);
void print_AI(struct AI*);
void randomize_AI_weights(struct AI*);
struct AI* create_AI_from_txt(char*);
void save_AI_to_txt(struct AI*, char*);
struct matrix* calculate_deltas(struct AI*, struct vector*, struct vector*, struct vector*);
void train(struct AI*, char*);
int load_from_locator(struct vector**, struct vector**, char*);
struct vector* feed_forward(struct vector**, struct vector*, struct AI*);
struct vector** back_propogation(struct vector*, struct AI*);
void adding_deltas(struct matrix**, struct vector*, struct vector**, struct vector**, struct vector*, struct vector*, struct AI*);

#endif