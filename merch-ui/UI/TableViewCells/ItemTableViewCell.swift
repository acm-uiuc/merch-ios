//
//  ItemTableViewCell.swift
//  merch-ui
//
//  Created by Rauhul Varma on 3/21/17.
//  Copyright © 2017 acm. All rights reserved.
//

import UIKit

class ItemTableViewCell: UITableViewCell {
    static let unselectedImage  = #imageLiteral(resourceName: "keypad_button_unselected")
    static let selectedImage    = #imageLiteral(resourceName: "keypad_button_selected")
    static let highlightedImage = #imageLiteral(resourceName: "keypad_button_highlighted")
    static let disabledImage    = #imageLiteral(resourceName: "keypad_button_disabled")
    
    // MARK: - Outlets
    @IBOutlet weak var iconImageView: UIImageView!
    @IBOutlet private weak var backgroundImageView: UIImageView!

    @IBOutlet weak var titleLabel: UILabel!
    @IBOutlet weak var costLabel: UILabel!
    
    @IBOutlet weak var hitBox: UIView!
    
    // MARK: - UIView
    override func point(inside point: CGPoint, with event: UIEvent?) -> Bool {
        return hitBox.point(inside: convert(point, to: hitBox), with: event)
    }
    
    // MARK: - UITableViewCell
    override func setSelected(_ selected: Bool, animated: Bool) {
        if selected {
            titleLabel.textColor = UIConstants.Colors.white
            costLabel.textColor = UIConstants.Colors.white
            backgroundImageView.image = ItemTableViewCell.selectedImage
        } else {
            titleLabel.textColor = UIConstants.Colors.primary
            costLabel.textColor = UIConstants.Colors.primary
            backgroundImageView.image = ItemTableViewCell.unselectedImage
        }
    }
    
    override func setHighlighted(_ highlighted: Bool, animated: Bool) {
        if highlighted {
            titleLabel.textColor = UIConstants.Colors.white
            costLabel.textColor = UIConstants.Colors.white
            backgroundImageView.image = ItemTableViewCell.highlightedImage
        } else {
            titleLabel.textColor = UIConstants.Colors.primary
            costLabel.textColor = UIConstants.Colors.primary
            backgroundImageView.image = ItemTableViewCell.unselectedImage
        }
    }
    
}
