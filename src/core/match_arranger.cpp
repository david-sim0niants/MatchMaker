#include "core/match_arranger.h"

#include <cassert>

namespace matchmaker::core {

std::unique_ptr<Match> MatchArranger::find_or_request_match(
        Player *player, Rating player_rating, const Game *game)
{
    if (has_arranged_match_for(player))
        throw MatchArrangerException("player has already requested a match");

    MatchByRating& match_by_rating = match_by_rating_by_game[game];
    MatchByRatingIt match_it = try_find_fairest_match(player_rating, match_by_rating);

    if (match_it == match_by_rating.end()) {
        arrange_new_match(player, player_rating, game);
        return nullptr;
    } else {
        return release_match(player, game, match_it);
    }
}

void MatchArranger::withdraw_match(const Player *player)
{
    if ( ! has_arranged_match_for(player))
        throw MatchArrangerException("player has not requested any match");
    else
        remove_match(player);
}

MatchArranger::MatchRequest *
    MatchArranger::get_match_request(const Player *player)
{
    auto match_request_it = match_request_by_player.find(player);
    if (match_request_it == match_request_by_player.end())
        return nullptr;
    else
        return &match_request_it->second;
}

const MatchArranger::MatchRequest *
    MatchArranger::get_match_request(const Player *player) const
{
    auto match_request_it = match_request_by_player.find(player);
    if (match_request_it == match_request_by_player.end())
        return nullptr;
    else
        return &match_request_it->second;
}

MatchArranger::MatchByRatingIt
    MatchArranger::try_find_fairest_match(Rating rating, MatchByRating& match_by_rating)
{
    auto match = match_by_rating.lower_bound(rating);
    auto higher_match = match_by_rating.upper_bound(rating);

    if (match != higher_match) {
        return match;
    }

    if (match != match_by_rating.begin()) {
        --match;
        const Rating match_rating = match->first.rating;
        if (match_rating + lower_rating_diff_thresh < rating)
            match = higher_match;
    }

    if (higher_match != match_by_rating.end()) {
        const Rating match_rating = match->first.rating;
        const Rating higher_match_rating = higher_match->first.rating;

        if (rating + higher_rating_diff_thresh < higher_match_rating) {
            if (match == higher_match)
                match = match_by_rating.end();
        } else if (match != higher_match) {
            if (higher_match_rating - rating < rating - match_rating)
                match = higher_match;
        }
    }

    return match;
}

void MatchArranger::arrange_new_match(Player *player, Rating rating, const Game *game)
{
    auto it = match_request_by_player.emplace(player, MatchRequest(player, rating, game)).first;
    match_by_rating_by_game[game][{rating, player}] = it->second.inactive_match.get();
}

std::unique_ptr<Match> MatchArranger::release_match(
        Player *player, const Game *game, MatchByRatingIt match_it)
{
    Player *match_opener = match_it->second->get_player_a();
    assert(match_opener != nullptr);

    match_by_rating_by_game[game].erase(match_it);

    MatchRequest match_request = remove_match_request(match_opener);
    match_request.inactive_match->add_player(*player);
    return std::move(match_request.inactive_match);
}

void MatchArranger::remove_match(const Player *player)
{
    MatchRequest match_request = remove_match_request(player);
    match_by_rating_by_game[match_request.get_game()].erase({match_request.rating, player});
}

MatchArranger::MatchRequest MatchArranger::remove_match_request(const Player *player)
{
    auto match_request_it = match_request_by_player.find(player);
    MatchRequest match_request = std::move(match_request_it->second);
    match_request_by_player.erase(match_request_it);
    return std::move(match_request);
}

}
