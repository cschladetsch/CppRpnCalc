#include <gtest/gtest.h>
#include "../src/Model/CalculatorModel.h"
#include <cmath>

class CalculatorModelTest : public ::testing::Test {
protected:
    CalculatorModel calc;
    
    void SetUp() override {
        calc.clear();
    }
};

TEST_F(CalculatorModelTest, PushAndPopValues) {
    calc.pushValue(5.0);
    calc.pushValue(10.0);
    
    EXPECT_EQ(calc.getStack().size(), 2);
    EXPECT_EQ(calc.getStack()[0], 5.0);
    EXPECT_EQ(calc.getStack()[1], 10.0);
    
    double value;
    EXPECT_TRUE(calc.popValue(value));
    EXPECT_EQ(value, 10.0);
    EXPECT_EQ(calc.getStack().size(), 1);
}

TEST_F(CalculatorModelTest, StackOperations) {
    // Test Drop
    calc.pushValue(1.0);
    calc.pushValue(2.0);
    calc.drop();
    EXPECT_EQ(calc.getStack().size(), 1);
    EXPECT_EQ(calc.getStack()[0], 1.0);
    
    // Test Swap
    calc.pushValue(3.0);
    calc.swap();
    EXPECT_EQ(calc.getStack().size(), 2);
    EXPECT_EQ(calc.getStack()[0], 3.0);
    EXPECT_EQ(calc.getStack()[1], 1.0);
    
    // Test Clear
    calc.clear();
    EXPECT_TRUE(calc.getStack().empty());
}

TEST_F(CalculatorModelTest, DuplicateOperation) {
    calc.pushValue(42.0);
    calc.duplicate();
    EXPECT_EQ(calc.getStack().size(), 2);
    EXPECT_EQ(calc.getStack()[0], 42.0);
    EXPECT_EQ(calc.getStack()[1], 42.0);
}

TEST_F(CalculatorModelTest, BasicArithmetic) {
    // Addition
    calc.pushValue(5.0);
    calc.pushValue(3.0);
    EXPECT_TRUE(calc.executeOperation("+"));
    EXPECT_EQ(calc.getStack().size(), 1);
    EXPECT_EQ(calc.getStack()[0], 8.0);
    
    // Subtraction
    calc.pushValue(2.0);
    EXPECT_TRUE(calc.executeOperation("-"));
    EXPECT_EQ(calc.getStack()[0], 6.0);
    
    // Multiplication
    calc.pushValue(4.0);
    EXPECT_TRUE(calc.executeOperation("*"));
    EXPECT_EQ(calc.getStack()[0], 24.0);
    
    // Division
    calc.pushValue(6.0);
    EXPECT_TRUE(calc.executeOperation("/"));
    EXPECT_EQ(calc.getStack()[0], 4.0);
}

TEST_F(CalculatorModelTest, DivisionByZero) {
    calc.pushValue(10.0);
    calc.pushValue(0.0);
    EXPECT_FALSE(calc.executeOperation("/"));
    EXPECT_TRUE(calc.hasError());
    EXPECT_EQ(calc.getError(), "Division by zero");
    
    // Stack should remain unchanged
    EXPECT_EQ(calc.getStack().size(), 2);
    EXPECT_EQ(calc.getStack()[0], 10.0);
    EXPECT_EQ(calc.getStack()[1], 0.0);
}

TEST_F(CalculatorModelTest, TrigonometricFunctions) {
    // Sin
    calc.pushValue(0.0);
    EXPECT_TRUE(calc.executeOperation("sin"));
    EXPECT_NEAR(calc.getStack()[0], 0.0, 1e-10);
    
    // Cos
    calc.clear();
    calc.pushValue(0.0);
    EXPECT_TRUE(calc.executeOperation("cos"));
    EXPECT_NEAR(calc.getStack()[0], 1.0, 1e-10);
    
    // Tan
    calc.clear();
    calc.pushValue(M_PI / 4);
    EXPECT_TRUE(calc.executeOperation("tan"));
    EXPECT_NEAR(calc.getStack()[0], 1.0, 1e-10);
}

TEST_F(CalculatorModelTest, SquareRoot) {
    calc.pushValue(16.0);
    EXPECT_TRUE(calc.executeOperation("sqrt"));
    EXPECT_EQ(calc.getStack()[0], 4.0);
    
    // Negative square root
    calc.clear();
    calc.pushValue(-4.0);
    EXPECT_FALSE(calc.executeOperation("sqrt"));
    EXPECT_TRUE(calc.hasError());
    EXPECT_EQ(calc.getError(), "Square root of negative number");
}

TEST_F(CalculatorModelTest, PowerOperation) {
    calc.pushValue(2.0);
    calc.pushValue(3.0);
    EXPECT_TRUE(calc.executeOperation("^"));
    EXPECT_EQ(calc.getStack()[0], 8.0);
}

TEST_F(CalculatorModelTest, ReciprocaAndNegate) {
    // 1/x
    calc.pushValue(4.0);
    EXPECT_TRUE(calc.executeOperation("1/x"));
    EXPECT_EQ(calc.getStack()[0], 0.25);
    
    // 1/0 should fail
    calc.clear();
    calc.pushValue(0.0);
    EXPECT_FALSE(calc.executeOperation("1/x"));
    EXPECT_TRUE(calc.hasError());
    
    // +/- (negate)
    calc.clear();
    calc.pushValue(5.0);
    EXPECT_TRUE(calc.executeOperation("+/-"));
    EXPECT_EQ(calc.getStack()[0], -5.0);
}

TEST_F(CalculatorModelTest, LogarithmicFunctions) {
    // Natural log
    calc.pushValue(M_E);
    EXPECT_TRUE(calc.executeOperation("ln"));
    EXPECT_NEAR(calc.getStack()[0], 1.0, 1e-10);
    
    // Log base 10
    calc.clear();
    calc.pushValue(100.0);
    EXPECT_TRUE(calc.executeOperation("log"));
    EXPECT_NEAR(calc.getStack()[0], 2.0, 1e-10);
    
    // Log of negative should fail
    calc.clear();
    calc.pushValue(-1.0);
    EXPECT_FALSE(calc.executeOperation("ln"));
    EXPECT_TRUE(calc.hasError());
}

TEST_F(CalculatorModelTest, InputBuffer) {
    calc.appendToInput("3");
    calc.appendToInput(".");
    calc.appendToInput("1");
    calc.appendToInput("4");
    EXPECT_EQ(calc.getInputBuffer(), "3.14");
    
    calc.backspace();
    EXPECT_EQ(calc.getInputBuffer(), "3.1");
    
    EXPECT_TRUE(calc.enterInput());
    EXPECT_EQ(calc.getStack().size(), 1);
    EXPECT_NEAR(calc.getStack()[0], 3.1, 1e-10);
    EXPECT_TRUE(calc.getInputBuffer().empty());
}

TEST_F(CalculatorModelTest, InvalidInput) {
    calc.setInputBuffer("abc");
    EXPECT_FALSE(calc.enterInput());
    EXPECT_TRUE(calc.hasError());
    EXPECT_EQ(calc.getError(), "Invalid input: abc");
}

TEST_F(CalculatorModelTest, EnterWithEmptyBufferDuplicates) {
    calc.pushValue(7.0);
    calc.clearInput();
    EXPECT_TRUE(calc.enterInput());
    EXPECT_EQ(calc.getStack().size(), 2);
    EXPECT_EQ(calc.getStack()[0], 7.0);
    EXPECT_EQ(calc.getStack()[1], 7.0);
}

TEST_F(CalculatorModelTest, StackUnderflow) {
    // Binary operation with one value
    calc.pushValue(5.0);
    EXPECT_FALSE(calc.executeOperation("+"));
    EXPECT_TRUE(calc.hasError());
    EXPECT_EQ(calc.getError(), "Need at least 2 values on stack");
    
    // Unary operation with empty stack
    calc.clear();
    EXPECT_FALSE(calc.executeOperation("sin"));
    EXPECT_TRUE(calc.hasError());
    EXPECT_EQ(calc.getError(), "Need at least 1 value on stack");
}

TEST_F(CalculatorModelTest, UnknownOperation) {
    calc.pushValue(1.0);
    EXPECT_FALSE(calc.executeOperation("unknown"));
    EXPECT_TRUE(calc.hasError());
    EXPECT_EQ(calc.getError(), "Unknown operation: unknown");
}

TEST_F(CalculatorModelTest, ErrorClearing) {
    calc.pushValue(0.0);
    calc.executeOperation("1/x");
    EXPECT_TRUE(calc.hasError());
    
    calc.clearError();
    EXPECT_FALSE(calc.hasError());
    EXPECT_TRUE(calc.getError().empty());
}

TEST_F(CalculatorModelTest, HistoryTracking) {
    calc.setInputBuffer("10");
    calc.enterInput();
    calc.setInputBuffer("20");
    calc.enterInput();
    calc.executeOperation("+");
    
    const auto& history = calc.getHistory();
    EXPECT_GE(history.size(), 3);
    // Should contain "10", "20", and "+"
}

TEST_F(CalculatorModelTest, FunctionDefinition) {
    calc.setInputBuffer("square { dup * }");
    EXPECT_TRUE(calc.enterInput());
    EXPECT_TRUE(calc.isFunctionDefined("square"));
    EXPECT_FALSE(calc.hasError());
}

TEST_F(CalculatorModelTest, FunctionExecution) {
    std::vector<std::string> body = {"dup", "*"};
    calc.defineFunction("square", body);
    
    calc.pushValue(5.0);
    EXPECT_TRUE(calc.executeFunction("square"));
    EXPECT_EQ(calc.getStack().size(), 1);
    EXPECT_EQ(calc.getStack().back(), 25.0);
}

TEST_F(CalculatorModelTest, FunctionCallViaOperation) {
    std::vector<std::string> body = {"2", "*"};
    calc.defineFunction("double", body);
    
    calc.pushValue(7.0);
    EXPECT_TRUE(calc.executeOperation("double"));
    EXPECT_EQ(calc.getStack().size(), 1);
    EXPECT_EQ(calc.getStack().back(), 14.0);
}

TEST_F(CalculatorModelTest, NestedFunctionCalls) {
    std::vector<std::string> doubleBody = {"2", "*"};
    std::vector<std::string> quadBody = {"double", "double"};
    
    calc.defineFunction("double", doubleBody);
    calc.defineFunction("quad", quadBody);
    
    calc.pushValue(3.0);
    EXPECT_TRUE(calc.executeFunction("quad"));
    EXPECT_EQ(calc.getStack().size(), 1);
    EXPECT_EQ(calc.getStack().back(), 12.0);
}

TEST_F(CalculatorModelTest, ComparisonOperations) {
    // Greater than
    calc.pushValue(5.0);
    calc.pushValue(3.0);
    EXPECT_TRUE(calc.executeOperation(">"));
    EXPECT_EQ(calc.getStack().back(), 1.0);
    
    // Less than
    calc.clear();
    calc.pushValue(3.0);
    calc.pushValue(5.0);
    EXPECT_TRUE(calc.executeOperation("<"));
    EXPECT_EQ(calc.getStack().back(), 1.0);
    
    // Equal
    calc.clear();
    calc.pushValue(5.0);
    calc.pushValue(5.0);
    EXPECT_TRUE(calc.executeOperation("=="));
    EXPECT_EQ(calc.getStack().back(), 1.0);
    
    // Not equal
    calc.clear();
    calc.pushValue(5.0);
    calc.pushValue(3.0);
    EXPECT_TRUE(calc.executeOperation("!="));
    EXPECT_EQ(calc.getStack().back(), 1.0);
    
    // Greater than or equal
    calc.clear();
    calc.pushValue(5.0);
    calc.pushValue(5.0);
    EXPECT_TRUE(calc.executeOperation(">="));
    EXPECT_EQ(calc.getStack().back(), 1.0);
    
    // Less than or equal
    calc.clear();
    calc.pushValue(3.0);
    calc.pushValue(5.0);
    EXPECT_TRUE(calc.executeOperation("<="));
    EXPECT_EQ(calc.getStack().back(), 1.0);
}

TEST_F(CalculatorModelTest, StackOperationsRot) {
    calc.pushValue(1.0);
    calc.pushValue(2.0);
    calc.pushValue(3.0);
    EXPECT_TRUE(calc.executeOperation("rot"));
    EXPECT_EQ(calc.getStack().size(), 3);
    EXPECT_EQ(calc.getStack()[0], 2.0);
    EXPECT_EQ(calc.getStack()[1], 3.0);
    EXPECT_EQ(calc.getStack()[2], 1.0);
}

TEST_F(CalculatorModelTest, StackOperationsOver) {
    calc.pushValue(1.0);
    calc.pushValue(2.0);
    EXPECT_TRUE(calc.executeOperation("over"));
    EXPECT_EQ(calc.getStack().size(), 3);
    EXPECT_EQ(calc.getStack()[0], 1.0);
    EXPECT_EQ(calc.getStack()[1], 2.0);
    EXPECT_EQ(calc.getStack()[2], 1.0);
}

TEST_F(CalculatorModelTest, StackOperationsPick) {
    calc.pushValue(10.0);
    calc.pushValue(20.0);
    calc.pushValue(30.0);
    calc.pushValue(1.0); // Pick index 1 (20.0)
    EXPECT_TRUE(calc.executeOperation("pick"));
    EXPECT_EQ(calc.getStack().size(), 4);
    EXPECT_EQ(calc.getStack().back(), 20.0);
}

TEST_F(CalculatorModelTest, StackOperationsRoll) {
    calc.pushValue(1.0);
    calc.pushValue(2.0);
    calc.pushValue(3.0);
    calc.pushValue(4.0);
    calc.pushValue(3.0); // Roll 3 items
    EXPECT_TRUE(calc.executeOperation("roll"));
    EXPECT_EQ(calc.getStack().size(), 4);
    EXPECT_EQ(calc.getStack()[0], 1.0);
    EXPECT_EQ(calc.getStack()[1], 4.0);
    EXPECT_EQ(calc.getStack()[2], 2.0);
    EXPECT_EQ(calc.getStack()[3], 3.0);
}

TEST_F(CalculatorModelTest, MathOperations) {
    // Absolute value
    calc.pushValue(-5.0);
    EXPECT_TRUE(calc.executeOperation("abs"));
    EXPECT_EQ(calc.getStack().back(), 5.0);
    
    // Modulo
    calc.clear();
    calc.pushValue(10.0);
    calc.pushValue(3.0);
    EXPECT_TRUE(calc.executeOperation("mod"));
    EXPECT_NEAR(calc.getStack().back(), 1.0, 1e-10);
    
    // Round
    calc.clear();
    calc.pushValue(3.7);
    EXPECT_TRUE(calc.executeOperation("round"));
    EXPECT_EQ(calc.getStack().back(), 4.0);
    
    // Floor
    calc.clear();
    calc.pushValue(3.7);
    EXPECT_TRUE(calc.executeOperation("floor"));
    EXPECT_EQ(calc.getStack().back(), 3.0);
    
    // Ceil
    calc.clear();
    calc.pushValue(3.2);
    EXPECT_TRUE(calc.executeOperation("ceil"));
    EXPECT_EQ(calc.getStack().back(), 4.0);
    
    // Min
    calc.clear();
    calc.pushValue(5.0);
    calc.pushValue(3.0);
    EXPECT_TRUE(calc.executeOperation("min"));
    EXPECT_EQ(calc.getStack().back(), 3.0);
    
    // Max
    calc.clear();
    calc.pushValue(5.0);
    calc.pushValue(3.0);
    EXPECT_TRUE(calc.executeOperation("max"));
    EXPECT_EQ(calc.getStack().back(), 5.0);
}

TEST_F(CalculatorModelTest, EdgeCasesForNewOperations) {
    // Pick with invalid index
    calc.clear();
    calc.pushValue(10.0);
    calc.pushValue(5.0); // Index out of bounds
    EXPECT_FALSE(calc.executeOperation("pick"));
    EXPECT_TRUE(calc.hasError());
    
    // Roll with invalid count
    calc.clear();
    calc.pushValue(1.0);
    calc.pushValue(5.0); // Count > stack size
    EXPECT_FALSE(calc.executeOperation("roll"));
    EXPECT_TRUE(calc.hasError());
    
    // Rot with insufficient stack
    calc.clear();
    calc.pushValue(1.0);
    calc.pushValue(2.0);
    EXPECT_FALSE(calc.executeOperation("rot"));
    EXPECT_TRUE(calc.hasError());
    
    // Over with insufficient stack
    calc.clear();
    calc.pushValue(1.0);
    EXPECT_FALSE(calc.executeOperation("over"));
    EXPECT_TRUE(calc.hasError());
    
    // Modulo by zero
    calc.clear();
    calc.pushValue(10.0);
    calc.pushValue(0.0);
    EXPECT_FALSE(calc.executeOperation("mod"));
    EXPECT_TRUE(calc.hasError());
}

TEST_F(CalculatorModelTest, ComplexFunctionWithNewOps) {
    // Define a function that uses comparison and stack operations
    std::vector<std::string> isPositiveBody = {"0", ">"};
    calc.defineFunction("isPositive", isPositiveBody);
    
    // Test with positive number
    calc.pushValue(5.0);
    EXPECT_TRUE(calc.executeOperation("isPositive"));
    EXPECT_EQ(calc.getStack().back(), 1.0);
    
    // Test with negative number
    calc.clear();
    calc.pushValue(-3.0);
    EXPECT_TRUE(calc.executeOperation("isPositive"));
    EXPECT_EQ(calc.getStack().back(), 0.0);
    
    // Define a function that finds the maximum of 3 numbers
    std::vector<std::string> max3Body = {"max", "max"};
    calc.defineFunction("max3", max3Body);
    
    calc.clear();
    calc.pushValue(5.0);
    calc.pushValue(10.0);
    calc.pushValue(3.0);
    EXPECT_TRUE(calc.executeOperation("max3"));
    EXPECT_EQ(calc.getStack().back(), 10.0);
}