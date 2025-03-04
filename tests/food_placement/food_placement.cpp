#include <gtest/gtest.h>
#include "Food.h"
#include "Snake.h"
#include <deque>
#include <memory>

#include <glm/glm.hpp>

// Test fixture
class Food_Test : public ::testing::Test
{
protected:
    std::unique_ptr<Food> food;
    std::unique_ptr<Snake> snake;

    void SetUp() override {
        std::deque<glm::ivec2> snakeBody = {{1, 1}, {2, 1}, {3,1}};
        food = std::make_unique<Food>(25, 22, 32, snakeBody);
        snake = std::make_unique<Snake>(nullptr, 25, 22, 32);
    }
    void TearDown() override {
    }
    
};

TEST_F(Food_Test, FoodShouldNotBePlacedOnSnake)
{
    for (int i = 0; i < 100; ++i)
    {
        food->respawn();
        

        bool isOnTheSnake = false;
        for (const auto& segment : snake->getSnake())
        {
            if (food->getPosition() == segment)
            {
                isOnTheSnake = true;
                break;
            }
        }
        EXPECT_FALSE(isOnTheSnake) << "Food was placed at the snake at position (" << food->getPosition().x << " , " << food->getPosition().y << ")\n";
    }
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
