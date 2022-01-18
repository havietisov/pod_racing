#include <cmath>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <algorithm>


using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

struct IntVec3
{
    float _x, _y, _z;
    IntVec3() : IntVec3(0,0,0){};
    IntVec3(float x, float y, float z) : _x(x), _y(y), _z(z) {}; 
};

struct IntVec2;
IntVec2 operator - (const IntVec2& a, const IntVec2& b);
IntVec2 operator + (const IntVec2& a, const IntVec2& b);

struct IntVec2
{
    float _x, _y;
    IntVec2(): IntVec2(0,0){}
    IntVec2(float x, float y) : _x(x), _y(y) {  }    

    float Magnitude() const
    {
        return sqrt(_x * _x + _y * _y);
    }

    static float dot(const IntVec2& a, const IntVec2& b)
    {
        return a._x * b._x + a._y * b._y;
    }

    static IntVec3 Cross(const IntVec2& a, const IntVec2& b)
    {
        return IntVec3(0, 0, a._x * b._y - a._y * b._x);//later
    }

    static float AngleBetweenDeg(const IntVec2& a, const IntVec2& b)
    {
        float dot = IntVec2::dot(a,b);
        float mag_1  = a.Magnitude();
        float mag_2  = b.Magnitude();

        auto diff = b - a;
        return std::acos(dot / (mag_1 * mag_2)) / M_PI * 180;
    }

    IntVec2 Normalized()
    {
        float mag = Magnitude();
        return IntVec2(((float)_x) / mag, ((float)_y) / mag);
    }

    IntVec2 NormalizedOrDefault()
    {
        float mag = Magnitude();
        return mag != 0 ? IntVec2(((float)_x) / mag, ((float)_y) / mag) : IntVec2(0,0);
    }
};

IntVec2 operator - (const IntVec2& a, const IntVec2& b)
{
    return IntVec2(a._x - b._x, a._y - b._y);
} 

IntVec2 operator + (const IntVec2& a, const IntVec2& b)
{
    return IntVec2(a._x + b._x, a._y + b._y);
} 

IntVec2 operator * (const IntVec2& a, float b)
{
    return IntVec2(a._x * b, a._y * b);
} 

IntVec2 operator * (float b, const IntVec2& a)
{
    return IntVec2(a._x * b, a._y * b);
} 

IntVec2 operator / (const IntVec2& a, float b)
{
    return IntVec2(a._x / b, a._y / b);
} 

struct PodContext
{
    IntVec2 position;
    float angle;
    IntVec2 position_d1;
    int checkpoint_id;
    bool boost_used;
    int shield_cooldown;
    int lap_number;

    IntVec2 GetLookVector()
    {
        return IntVec2(cos(angle / 180.0 * M_PI), sin(angle / 180.0 * M_PI));
    }

    IntVec2 target_direction;
    float angle_face_against_target;
    float angle_face_against_motion;
    float angle_motion_against_target;
    IntVec2 next_pos_approx;

    bool CollidesWith(IntVec2 position)
    {
        return (position - this->position).Magnitude() <= 850;       
    }

    bool CollidesWithAtNextFrame(IntVec2 position, float ref_dist = 850)
    {
        return (position - this->next_pos_approx).Magnitude() <= ref_dist;     
    }
};

void EnrichContext(PodContext& ctx, std::vector<IntVec2> checkpoints)
{
    ctx.target_direction = checkpoints[ctx.checkpoint_id] - ctx.position;
    ctx.angle_face_against_target = IntVec2::AngleBetweenDeg(ctx.GetLookVector(), ctx.target_direction);
    ctx.angle_face_against_motion = IntVec2::AngleBetweenDeg(ctx.GetLookVector(), ctx.position_d1);
    ctx.angle_motion_against_target = IntVec2::AngleBetweenDeg(ctx.position_d1, ctx.target_direction);
    ctx.next_pos_approx = ctx.position_d1 + ctx.position;
}

int main()
{
    int laps;
    cin >> laps; cin.ignore();
    int checkpoint_count;

    std::vector<IntVec2> checkpoints;

    cin >> checkpoint_count; cin.ignore();
    for (int i = 0; i < checkpoint_count; i++) {
        int checkpoint_x;
        int checkpoint_y;
        cin >> checkpoint_x >> checkpoint_y; cin.ignore();
        checkpoints.push_back(IntVec2(checkpoint_x, checkpoint_y));
    }

    checkpoints.push_back(checkpoints[0]);
    
    // game loop
    while (1) {

        std::array<PodContext, 2> contexts;
        contexts[0].shield_cooldown = 0;
        contexts[1].shield_cooldown = 0;
        std::array<PodContext, 2> enemy_contexts;

        for (int i = 0; i < 2; i++) {
            int x; // x position of your pod
            int y; // y position of your pod
            int vx; // x speed of your pod
            int vy; // y speed of your pod
            int angle; // angle of your pod
            int next_check_point_id; // next check point id of your pod
            cin >> x >> y >> vx >> vy >> angle >> next_check_point_id; cin.ignore();

            if(contexts[i].checkpoint_id > next_check_point_id)
            {
                contexts[i].lap_number++;
            }

            contexts[i].angle = angle;
            contexts[i].checkpoint_id = next_check_point_id;
            contexts[i].position = IntVec2(x, y);
            contexts[i].position_d1 = IntVec2(vx, vy);
            EnrichContext(contexts[i], checkpoints);
        }
        for (int i = 0; i < 2; i++) {
            int x_2; // x position of the opponent's pod
            int y_2; // y position of the opponent's pod
            int vx_2; // x speed of the opponent's pod
            int vy_2; // y speed of the opponent's pod
            int angle_2; // angle of the opponent's pod
            int next_check_point_id_2; // next check point id of the opponent's pod
            cin >> x_2 >> y_2 >> vx_2 >> vy_2 >> angle_2 >> next_check_point_id_2; cin.ignore();
            
            if(enemy_contexts[i].checkpoint_id > next_check_point_id_2)
            {
                enemy_contexts[i].lap_number++;
            }

            enemy_contexts[i].angle = angle_2;
            enemy_contexts[i].checkpoint_id = next_check_point_id_2;
            enemy_contexts[i].position = IntVec2(x_2, y_2);
            enemy_contexts[i].position_d1 = IntVec2(vx_2, vy_2);
            EnrichContext(enemy_contexts[i], checkpoints);
        }

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;


        // You have to output the target position
        // followed by the power (0 <= thrust <= 100)
        // i.e.: "x y thrust"

        for(int i = 0; i < 2; i ++)
        {
            auto current_player = contexts[i];
            IntVec2 current_cp_pos = checkpoints[current_player.checkpoint_id];
            IntVec2 target_pos = checkpoints[current_player.checkpoint_id] - current_player.position_d1 * 3;
            IntVec2 diff = target_pos - current_player.position;
            float next_checkpoint_dist = diff.Magnitude();
            float target_thrust = 100;
            float enemy_dist_min = std::min((current_player.position - enemy_contexts[0].position).Magnitude(), (current_player.position - enemy_contexts[1].position).Magnitude());
            
            float motion_to_target_dist = (diff - (IntVec2::dot(diff, current_player.position_d1.NormalizedOrDefault()) * current_player.position_d1.Normalized())).Magnitude();

            if(motion_to_target_dist < 300 && diff.Magnitude() < current_player.position_d1.Magnitude() * 2 )
            {
                target_pos = checkpoints[current_player.checkpoint_id + 1] - current_player.position_d1 * 3;
                IntVec2 diff = target_pos - current_player.position;
                float _angle_to_target = IntVec2::AngleBetweenDeg( current_player.GetLookVector() , diff);

                cerr << "angle to chosen target " <<  _angle_to_target << endl;

                if (_angle_to_target > 35)
                {
                    target_thrust = 0;
                }
            }

            bool use_shield = false;

            float angle_to_target = IntVec2::AngleBetweenDeg( current_player.GetLookVector() , diff);
            diff = target_pos - current_player.position;
            float dot_velocity_against_diff = IntVec2::dot(current_player.position_d1.Normalized(), diff.Normalized());
            
            if ( angle_to_target > 55)
            {
                target_thrust = 50;
            }

            if ( angle_to_target > 75)
            {
                target_thrust = 0;
            }

            if(current_player.shield_cooldown > 0)
            {
                current_player.shield_cooldown--;
            }

            if ( i == 1)
            {
                auto chosen_enemy = enemy_contexts[0];

                if (enemy_contexts[1].lap_number == enemy_contexts[0].lap_number)
                {
                    if(enemy_contexts[1].checkpoint_id == enemy_contexts[0].checkpoint_id)
                    {
                        float dist_0 = (enemy_contexts[0].position - checkpoints[enemy_contexts[0].checkpoint_id]).Magnitude();
                        float dist_1 = (enemy_contexts[1].position - checkpoints[enemy_contexts[1].checkpoint_id]).Magnitude();

                        if (dist_0 < dist_1)
                        {
                            chosen_enemy = enemy_contexts[0];
                        }
                        else
                        {
                            chosen_enemy = enemy_contexts[1];
                        }
                    }
                    else
                    {
                        if (enemy_contexts[1].checkpoint_id > enemy_contexts[0].checkpoint_id)
                        {
                            chosen_enemy = enemy_contexts[1];
                        }
                        else
                        {
                            chosen_enemy = enemy_contexts[0];
                        }
                    }
                }
                else
                {
                    if (enemy_contexts[1].lap_number > enemy_contexts[0].lap_number)
                    {
                        chosen_enemy = enemy_contexts[1];
                    }
                    else
                    {
                        chosen_enemy = enemy_contexts[0];
                    }
                }
                
                IntVec2 enemy_pos;
                target_thrust = 100;

                auto mult = (chosen_enemy.position - current_player.position).Magnitude() / 1000.0f;
                mult = mult > 8 ? 8 : mult;
                
                enemy_pos = chosen_enemy.position + chosen_enemy.position_d1 * mult - current_player.position_d1;//getting reference pos 1
    
                target_pos = enemy_pos * 0.85f + checkpoints[chosen_enemy.checkpoint_id+1] * 0.15f;

                if((chosen_enemy.position - current_player.position).Magnitude() < 1500)
                {
                    target_pos = chosen_enemy.position + chosen_enemy.position_d1 * mult / 2;
                }

                auto allydiff = contexts[0].position - contexts[1].position;

                if (allydiff.Magnitude() < 2000 )
                {
                    target_pos = current_player.position - allydiff;
                }

                {
                    auto final_diff = target_pos - current_player.position ;

                    float final_angle = IntVec2::AngleBetweenDeg( current_player.GetLookVector() , final_diff);

                    if(final_angle > 45)
                    {
                        target_thrust = 0;
                    }
                }
            }

            bool allies_going_to_jam = contexts[1].CollidesWithAtNextFrame(contexts[0].next_pos_approx);

            float _1_dist_to_target = (enemy_contexts[0].position - current_cp_pos).Magnitude();
            float _2_dist_to_target = (enemy_contexts[1].position - current_cp_pos).Magnitude();
            float _my_dist_to_target = (current_player.position - current_cp_pos).Magnitude();
            float _ally_0_dist_to_target = (contexts[0].position - current_cp_pos).Magnitude();
            float _ally_1_dist_to_target = (contexts[1].position - current_cp_pos).Magnitude();

            bool _1_bump_favourable = _1_dist_to_target > _my_dist_to_target;
            bool _2_bump_favourable = _2_dist_to_target > _my_dist_to_target;
            bool _ally_bump_favourable = _ally_1_dist_to_target > _ally_0_dist_to_target;
            
            bool _1_moves_towards_me = -0.13 < IntVec2::dot((current_player.position_d1).Normalized(),  (enemy_contexts[0].position_d1).Normalized());
            bool _2_moves_towards_me = -0.13 < IntVec2::dot((current_player.position_d1).Normalized(),  (enemy_contexts[1].position_d1).Normalized());
            bool i_collide_with_1 = current_player.CollidesWithAtNextFrame(enemy_contexts[0].next_pos_approx);
            bool i_collide_with_2 = current_player.CollidesWithAtNextFrame(enemy_contexts[1].next_pos_approx);
            bool allies_collide = contexts[0].CollidesWithAtNextFrame(contexts[1].next_pos_approx);

            if(current_player.shield_cooldown == 0)
            {
                if (i == 0)
                {
                    if(!_1_bump_favourable && !_2_bump_favourable)
                    {
                        if (i_collide_with_1 || i_collide_with_2 || allies_collide)
                        {
                            if(i_collide_with_1 && _1_moves_towards_me)
                            {
                                use_shield = true;
                                current_player.shield_cooldown = 10;
                            }

                            if(i_collide_with_2 && _2_moves_towards_me)
                            {
                                use_shield = true;
                                current_player.shield_cooldown = 10;
                            }
                            
                            if(allies_collide)
                            {
                                use_shield = true;
                                current_player.shield_cooldown = 10;
                            }
                        }
                    }
                }
                else
                {
                    if(i_collide_with_1 || i_collide_with_2)
                    {
                        use_shield = true;
                    }
                }
            }

            if ( i == 0 && target_thrust == 0 && current_player.shield_cooldown == 0)//if i need to slow down, why not use shield?
            {
                if ( current_player.CollidesWithAtNextFrame(enemy_contexts[0].next_pos_approx) || current_player.CollidesWithAtNextFrame(enemy_contexts[1].next_pos_approx) )//apparently, using shield at each checkpoint is not a good strategy. However, it's not bad idea to use that shield when enemy might bump into us
                {
                    use_shield = true;
                    current_player.shield_cooldown = 10;
                }
            }

            if(i != 1 && !current_player.boost_used && next_checkpoint_dist > 4000 && target_thrust > 50)
            {
                cout << (int)target_pos._x << " " << (int)target_pos._y << " BOOST" << endl;
                current_player.boost_used = true;
            }
            else
            {
                if(!use_shield)
                {
                    cout << (int)target_pos._x << " " << (int)target_pos._y << " " << target_thrust << endl;
                }
                else
                {
                    cout << (int)target_pos._x << " " << (int)target_pos._y << " SHIELD" << std::endl;
                }
            }

            contexts[i] = current_player;
        }
    }
}
