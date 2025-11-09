using JuMP
import HiGHS

n_intervals = 4

production_cost = [6000, 4000, 8000, 9000]
max_overflow = [60, 65, 70, 60]
cost_overflow = [8000, 6000, 10000, 11000]
demand = [130, 80, 125, 195]
capacity = 70
I0 = 15

model = Model(HiGHS.Optimizer)

@variable(model, 0<= x[i in 1:n_intervals] <= 100)
@variable(model, 0<= y[i in 1:n_intervals] <= max_overflow[i])
@variable(model, 0<= I[i in 1:n_intervals] <=70)

@constraint(model, bilans_init, I[1] == I0 + x[1] + y[1] - demand[1])

@constraint(model, bilans[i in 2:n_intervals], I[i] == I[i-1] + x[i] + y[i] - demand[i])

@objective(model, Min, sum(x[i]*production_cost[i] + y[i]*cost_overflow[i] + 1500*I[i] for i in 1:n_intervals))


optimize!(model)

if termination_status(model) == MOI.OPTIMAL
    println("OPTIMAL")
else
    println("NON OPTIMAL: ", termination_status)
end