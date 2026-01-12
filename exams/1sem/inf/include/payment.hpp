#pragma once

#include <iostream>
#include <memory>
#include <pqxx/pqxx>

class PaymentStrategy
{
protected:
    float sum;

public:
    PaymentStrategy() { sum = 0; }
    PaymentStrategy(float sum) : sum(sum) {}
    virtual bool pay() = 0;
};

class CreditPayment : public PaymentStrategy
{
public:
    CreditPayment(float sum) : PaymentStrategy(sum) {}
    bool pay() override;
};
class CashPayment : public PaymentStrategy
{
public:
    CashPayment(float sum) : PaymentStrategy(sum) {}
    bool pay() override;
};
class FPS : public PaymentStrategy
{
public:
    FPS(float sum) : PaymentStrategy(sum) {}
    bool pay() override;
};
