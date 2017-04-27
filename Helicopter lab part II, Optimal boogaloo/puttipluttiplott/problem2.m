
figure(1)
plot(oppg2Q1_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out(1,:), oppg2Q1_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out(2,:),...
    oppg2Q1_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out(1,:), oppg2Q1_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out(7,:));
title('Q = 1, optimal and measured states');
xlabel('Time [s]');
ylabel('Angle [rad]');
legend('Optimal', 'Measured');

figure(2)
plot(oppg2Q01_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out(1,:), oppg2Q01_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out(7,:),...
    oppg2Q1_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out(1,:), oppg2Q1_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out(7,:),...
    oppg2Q10_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out(1,:),oppg2Q10_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out(7,:));
title('Q = 0.1, Q = 1 and Q = 10,  measured states');
xlabel('Time [s]');
ylabel('Angle [rad]');
legend('Q=0.1', 'Q=1', 'Q=10');

figure(3)
plot(oppg2Q01_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out(1,:), oppg2Q01_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out(2,:),...
    oppg2Q1_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out(1,:), oppg2Q1_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out(2,:),...
    oppg2Q10_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out(1,:),oppg2Q10_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out(2,:));
title('Q = 0.1, Q = 1 and Q = 10,  optimal states');
xlabel('Time [s]');
ylabel('Angle [rad]');
legend('Q=0.1', 'Q=1', 'Q=10');

figure(4)
plot(oppg2Q01_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out(1,:), oppg2Q01_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out(6,:),...
    oppg2Q1_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out(1,:), oppg2Q1_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out(6,:),...
    oppg2Q10_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out(1,:),oppg2Q10_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out(6,:));
title('Q = 0.1, Q = 1 and Q = 10,  Different optimal inputs');
xlabel('Time [s]');
ylabel('Angle [rad]');
legend('Q=0.1', 'Q=1', 'Q=10');
