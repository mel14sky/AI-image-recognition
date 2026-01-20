#ifndef AI_MATRIX_LOCATOR_H
#define AI_MATRIX_LOCATOR_H

struct vector;
struct matrix;
struct AI;

void print_v(const struct vector*);
void print_m(const struct matrix*);
void fill_random_v(const struct vector*);
void fill_random_m(const struct matrix*);
struct vector* make_vector(unsigned int);
struct matrix* make_matrix(unsigned int, unsigned int);
struct vector* make_vector_from_txt(char*);
void free_vector(struct vector*);
void free_matrix(struct matrix*);
void multiply_scalar_m(struct matrix* mat, double scalar);
struct vector* multiply(const struct matrix*, const struct vector*);
struct vector* multiply_add_bias(const struct matrix*, const struct vector*);
struct vector* multiply_elements_v_v(const struct vector*, const struct vector*);
struct matrix* multiply_v_vt(const struct vector*, const struct vector*, const struct AI*);
void multiply_scalar_v(const struct vector*, double);
struct matrix* divide_scalar_m(const struct matrix*, double);
void matrix_fill(const struct matrix* mat, double);
void matrix_add(const struct matrix*, const struct matrix*);
struct matrix* transpose_m(const struct matrix*);
struct vector* vector_subtract(const struct vector*, const struct vector*);
void activation_function(const struct vector*);
struct vector* derivative_activation_function(const struct vector*);
struct AI* create_AI(const int*, unsigned short, double);
void free_AI(struct AI*);
void print_AI(const struct AI*);
void randomize_AI_weights(const struct AI*);
struct AI* create_AI_from_txt(char*);
void save_AI_to_txt(const struct AI*, char*);
struct matrix* calculate_deltas(const struct AI*, const struct vector*, const struct vector*, const struct vector*);
//void train_single(const struct AI*, char*, char*);
void train(const struct AI*, char*);
struct training_data* load_from_locator(char*);
struct feed_forward_return* feed_forward(const struct vector*, const struct AI*);
struct vector** back_propagation(const struct vector*, const struct AI*);
void adding_deltas(struct matrix**, const struct vector*, struct feed_forward_return*, struct vector**, const struct vector*, const struct AI*);
void free_training_data(struct training_data* training_data);
void free_feed_forward_return(struct feed_forward_return* feed_forward_return, const struct AI* ai);


#endif //AI_MATRIX_LOCATOR_H