#include <algorithm>
#include "gtest/gtest.h"

extern "C" {
    #include "../src/fa.h"
}

/*
 *
 */

fa *faTest, *faTest2, *faTest3, *faTest4, *faTest5, *faTest6;
graph *grTest1, *grTest2, *grTest3;

size_t alpha_countTest = 2;
size_t state_countTest = 5;

/*
 * CREATE FUNCTIONS
 */

void __init()
{
    /* FA 1 */
    faTest = static_cast<fa *>(malloc(sizeof(fa)));         // On alloue la mémoire
    fa_create(faTest, alpha_countTest, state_countTest);    // On crée l'automate


    /* FA 2 */
    faTest2 = static_cast<fa *>(malloc(sizeof(fa)));
    fa_create(faTest2, 2, 3);

    fa_add_transition(faTest2, 0, 'a', 1);
    fa_add_transition(faTest2, 0, 'b', 2);
    fa_add_transition(faTest2, 1, 'c', 2);

    fa_set_state_initial(faTest2, 2);
    fa_set_state_final(faTest2, 2);

    /* FA 3 */
    faTest3 = static_cast<fa *>(malloc(sizeof(fa)));
    fa_create(faTest3, 1, 2);

    fa_add_transition(faTest3, 0, 'a', 1);
    fa_add_transition(faTest3, 1, 'a', 1);

    fa_set_state_initial(faTest3, 0);
    fa_set_state_final(faTest3, 1);


    /* FA 4 */
    faTest4 = static_cast<fa *>(malloc(sizeof(fa)));
    fa_create(faTest4, 2, 3);

    fa_add_transition(faTest4, 0, 'a', 1);
    fa_add_transition(faTest4, 1, 'b', 1);
    fa_add_transition(faTest4, 2, 'a', 1);

    fa_set_state_initial(faTest4, 0);
    fa_set_state_final(faTest4, 1);


    /* FA 5 */

    faTest5 = static_cast<fa *>(malloc(sizeof(fa)));

    fa_create(faTest5,2,5);

    fa_set_state_initial(faTest5,0);
    fa_set_state_initial(faTest5,1);
    fa_set_state_final(faTest5,1);
    fa_set_state_final(faTest5,4);

    fa_add_transition(faTest5,0,'a',1);
    fa_add_transition(faTest5,0,'a',2);
    fa_add_transition(faTest5,0,'a',3);
    fa_add_transition(faTest5,1,'b',3);
    fa_add_transition(faTest5,2,'a',3);
    fa_add_transition(faTest5,2,'b',4);
    fa_add_transition(faTest5,3,'a',3);
    fa_add_transition(faTest5,3,'b',4);
    fa_add_transition(faTest5,4,'a',4);

    /* FA 6 */
    faTest6 = static_cast<fa *>(malloc(sizeof(fa)));

    fa_create(faTest6,2,5);


}

TEST(fa_structure, fa_create_TEST)
{
    EXPECT_EQ(sizeof(faTest), 8);

    EXPECT_NE(sizeof(faTest), sizeof(fa));                  // On vérifie que la mémoire est bien allouée
    EXPECT_EQ(faTest->alpha_count, alpha_countTest);        // Vérification des valeurs passées 1.
    EXPECT_EQ(faTest->state_count, state_countTest);        // Vérification des valeurs passées 2.
}

TEST(fa_structure, fa_create_state_TEST)
{
    for ( int i = 0; i < faTest->state_count; i++ )
    {
        EXPECT_EQ(faTest->initial_states[i], false);
        EXPECT_EQ(faTest->final_states[i], false);
    }
}

TEST(fa_structure, fa_set_initial_state_TEST)
{
    fa_set_state_initial(faTest, 0);
    fa_set_state_initial(faTest, 1);

    EXPECT_EQ(faTest->initial_states[0], true);
    EXPECT_EQ(faTest->initial_states[1], true);
    EXPECT_NE(faTest->initial_states[2], true);
}

TEST(fa_structure, fa_set_final_state_TEST)
{
    fa_set_state_final(faTest, 1);
    fa_set_state_final(faTest, 4);

    EXPECT_EQ(faTest->final_states[1], true);
    EXPECT_EQ(faTest->final_states[4], true);
    EXPECT_NE(faTest->final_states[2], true);
}

TEST(fa_structure, fa_add_transition_TEST)
{
    fa_add_transition(faTest,0,'a',2);
    fa_add_transition(faTest,0,'a',3);
    fa_add_transition(faTest,0,'b',1);
    fa_add_transition(faTest,0,'b',2);

    state_node *transitions = faTest->transitions[0];

    EXPECT_EQ(transitions->state, 2);
    EXPECT_EQ(transitions->letter, 'a');

    transitions = transitions->next;
    EXPECT_EQ(transitions->state, 3);
    EXPECT_EQ(transitions->letter, 'a');

    transitions = transitions->next;
    EXPECT_EQ(transitions->state, 1);
    EXPECT_EQ(transitions->letter, 'b');

    transitions = transitions->next;
    EXPECT_EQ(transitions->state, 2);
    EXPECT_EQ(transitions->letter, 'b');

    transitions = transitions->next;
    EXPECT_EQ(transitions, nullptr);
}

TEST(fa_structure, fa_remove_transition_TEST)
{
    fa_remove_transition(faTest, 0, 'a', 2);

    state_node *transitions1 = faTest->transitions[0];
    EXPECT_NE(transitions1->state, 2);

    EXPECT_EQ(transitions1->state, 3);
    EXPECT_EQ(transitions1->letter, 'a');
}

TEST(fa_structure, fa_remove_state_TEST)
{
    EXPECT_EQ(faTest->state_count, state_countTest);

    fa_remove_state(faTest, 3);

    EXPECT_EQ(faTest->state_count, state_countTest-1);

    state_node *transitions2 = faTest->transitions[0];

    transitions2 = transitions2->next;

    EXPECT_NE(transitions2->letter, 'a');
    EXPECT_NE(transitions2->state, 3);

    EXPECT_EQ(transitions2->letter, 'b');
    EXPECT_EQ(transitions2->state, 2);

}

TEST(fa_structure, fa_count_transitions_TEST)
{
    EXPECT_EQ(fa_count_transitions(faTest), 2);
}

TEST(fa_structure, fa_is_deterministic_TEST)
{
    EXPECT_FALSE(fa_is_deterministic(faTest));
    EXPECT_TRUE(fa_is_deterministic(faTest2));
}

TEST(fa_structure, fa_is_complete_TEST)
{
    EXPECT_FALSE(fa_is_complete(faTest));
    EXPECT_TRUE(fa_is_complete(faTest3));

    fa_destroy(faTest3);
}

TEST(fa_structure, fa_make_complete_TEST)
{
    EXPECT_FALSE(fa_is_complete(faTest2));
    fa_make_complete(faTest2);
    EXPECT_TRUE(faTest2);
}

TEST(graph_structure, graph_create_from_fa_TEST)
{
    grTest1 = static_cast<graph *>(malloc(sizeof(graph)));
    grTest2 = static_cast<graph *>(malloc(sizeof(graph)));
    graph_create_from_fa(grTest1, faTest4, false);
    graph_create_from_fa(grTest2, faTest5, false);

    EXPECT_EQ(sizeof(grTest1), 8);
    EXPECT_EQ(sizeof(grTest2), 8);
}

TEST(graph_structure, graph_has_path_TEST)
{
    EXPECT_FALSE(graph_has_path(grTest1, 0, 2));
    EXPECT_TRUE(graph_has_path(grTest2, 0, 2));
}

TEST(graph_structure, graph_destroy_TEST)
{
    graph_destroy(grTest1);
    graph_destroy(grTest2);

    EXPECT_EQ(sizeof(grTest1), 8);
    EXPECT_EQ(sizeof(grTest2), 8);
    EXPECT_EQ(grTest1->A, nullptr);
    EXPECT_EQ(grTest2->A, nullptr);

    fa_destroy(faTest4);
    fa_destroy(faTest5);
}


TEST(fa_structure, fa_is_language_empty_TEST)
{
    EXPECT_FALSE(fa_is_language_empty(faTest5));
    EXPECT_TRUE(fa_is_language_empty(faTest6));

    fa_destroy(faTest6);
}

TEST(fa_structure, fa_remove_non_accessible_states_TEST )
{
    EXPECT_EQ(faTest4->state_count, 3);
    fa_remove_non_accessible_states(faTest4);
    EXPECT_EQ(faTest4->state_count, 2);

}

TEST(fa_structure, fa_remove_non_co_accessible_states_TEST)
{

    fa *faTestNonCo = static_cast<fa *>(malloc(sizeof(fa)));
    fa_create(faTestNonCo, 2, 3);

    fa_add_transition(faTestNonCo, 0, 'a', 1);
    fa_add_transition(faTestNonCo, 1, 'b', 1);
    fa_add_transition(faTestNonCo, 2, 'a', 1);

    fa_set_state_initial(faTestNonCo, 0);
    fa_set_state_final(faTestNonCo, 1);

    EXPECT_EQ(faTestNonCo->state_count, 3);
    fa_remove_non_co_accessible_states(faTestNonCo);
    EXPECT_EQ(faTestNonCo->state_count, 2);

    fa_destroy(faTestNonCo);
}

TEST(fa_structure, fa_has_empty_intersection_TEST)
{
    EXPECT_FALSE(fa_has_empty_intersection(faTest, faTest2));
}

TEST(fa_structure, fa_create_deterministic)
{
    EXPECT_FALSE(fa_is_deterministic(faTest));
    fa *res = static_cast<fa *>(malloc(sizeof(fa)));
    fa_create_deterministic(faTest, res);
    EXPECT_TRUE(fa_is_deterministic(res));
}

TEST(fa_structure, fa_is_included_TEST)
{
    fa *fa1Test = static_cast<fa *>(malloc(sizeof(fa)));
    fa *fa2Test = static_cast<fa *>(malloc(sizeof(fa)));

    fa_create(fa1Test, 2, 4);
    fa_create(fa2Test, 2, 2);

    fa_add_transition(fa1Test, 0, 'a', 1);
    fa_add_transition(fa1Test, 1, 'b', 2);
    fa_add_transition(fa1Test, 2, 'a', 3);

    fa_add_transition(fa2Test, 0, 'b', 1);

    fa_set_state_initial(fa1Test, 0);
    fa_set_state_initial(fa2Test, 0);

    fa_set_state_final(fa1Test, 3);
    fa_set_state_final(fa2Test, 1);

    EXPECT_FALSE(fa_is_included(fa1Test, fa2Test));

    fa_destroy(fa1Test);
    fa_destroy(fa2Test);
}


TEST(fa_structure, fa_destroy_TEST)
{
    fa_destroy(faTest);
    fa_destroy(faTest2);

    EXPECT_EQ(sizeof(faTest), 8);                   // On vérifie que la mémoire n'est plus allouée
    EXPECT_EQ(sizeof(faTest2), 8);                   // On vérifie que la mémoire n'est plus allouée
    EXPECT_EQ(faTest->transitions, nullptr);
    EXPECT_EQ(faTest2->transitions, nullptr);
}


int main(int argc, char **argv) {
    __init();

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}