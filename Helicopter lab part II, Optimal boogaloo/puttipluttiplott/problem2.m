
figure(1)
plot(oppg2Q1_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out(1,:), oppg2Q1_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out(2,:),...
    oppg2Q1_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out(1,:), oppg2Q1_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out(7,:));
title('q = 1, optimal and measured states');
xlabel('Time [s]');
ylabel('Angle [rad]');
legend('Optimal', 'Measured');

figure(2)
plot(oppg2Q01_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out(1,:), oppg2Q01_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out(7,:),...
    oppg2Q1_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out(1,:), oppg2Q1_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out(7,:),...
    oppg2Q10_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out(1,:),oppg2Q10_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out(7,:));
title('q=0.1, q=1 and q=10, measured \lambda');
xlabel('Time [s]');
ylabel('Angle [rad]');
legend('q=0.1', 'q=1', 'q=10');

figure(3)
plot(oppg2Q01_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out(1,:), oppg2Q01_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out(2,:),...
    oppg2Q1_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out(1,:), oppg2Q1_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out(2,:),...
    oppg2Q10_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out(1,:),oppg2Q10_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out(2,:));
title('q=0.1, q=1 and q=10, optimal states');
xlabel('Time [s]');
ylabel('Angle [rad]');
legend('q=0.1', 'q=1', 'q=10');

figure(4)
plot(oppg2Q01_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out(1,:), oppg2Q01_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out(6,:),...
    oppg2Q1_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out(1,:), oppg2Q1_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out(6,:),...
    oppg2Q10_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out(1,:),oppg2Q10_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out(6,:));
title('q=0.1, q=1 and q=10, different optimal inputs');
xlabel('Time [s]');
ylabel('Angle [rad]');
legend('q=0.1', 'q=1', 'q=10');
