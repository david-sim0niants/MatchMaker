#include "core/match_arranger.h"

namespace matchmaker::core {

std::optional<Match> MatchArranger::find_or_request_match(
        Player& player, int player_rating, const Game& game)
{
    if (has_arranged_match_for(player))
        throw MatchArrangerException("player has already requested a match");

    MatchByRating& match_by_rating = match_by_rating_by_game[&game];
    MatchByRatingIt rating_it = try_find_fairest_match(player_rating, match_by_rating);

    if (rating_it == match_by_rating.end()) {
        arrange_new_match(player, player_rating, game);
        return std::nullopt;
    } else {
        return release_match(player, game, rating_it);
    }
}

void MatchArranger::withdraw_match(const Player& player)
{
    // TODO

}

const Game *MatchArranger::get_player_game(const Player& player) const
{
    auto game_it = game_by_player.find(&player);
    if (game_it == game_by_player.end())
        return nullptr;
    else
        return game_it->second;
}

MatchArranger::MatchByRatingIt
    MatchArranger::try_find_fairest_match(int player_rating, MatchByRating& match_by_rating)
{
    auto match = match_by_rating.lower_bound(player_rating);
    auto higher_match = match_by_rating.upper_bound(player_rating);

    if (match != higher_match) {
        return match;
    }

    if (match != match_by_rating.begin()) {
        --match;
        const int match_rating = match->first;
        if (match_rating + lower_rating_diff_thresh < player_rating)
            match = higher_match;
    }

    if (higher_match != match_by_rating.end()) {
        const int match_rating = match->first;
        const int higher_match_rating = higher_match->first;

        if (player_rating + higher_rating_diff_thresh < higher_match_rating) {
            if (match == higher_match)
                match = match_by_rating.end();
        } else if (match != higher_match) {
            if (higher_match_rating - player_rating < player_rating - match_rating)
                match = higher_match;
        }
    }

    return match;
}

void MatchArranger::arrange_new_match(Player& player, int rating, const Game& game)
{
    game_by_player.emplace(&player, &game);
    Match new_match;
    new_match.add_player(player);
    match_by_rating_by_game[&game].emplace(rating, new_match);
}

Match MatchArranger::release_match(Player& player, const Game& game, MatchByRatingIt rating_it)
{
    Match match = rating_it->second;
    match.add_player(player);
    match_by_rating_by_game[&game].erase(rating_it);
    game_by_player.erase(match.get_player_a());
    return match;
}

}
