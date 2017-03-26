//
//  ItemsViewController.swift
//  merch-ui
//
//  Created by Rauhul Varma on 3/21/17.
//  Copyright © 2017 acm. All rights reserved.
//

import UIKit

class ItemsViewController: UIViewController {

    // MARK: - Outlets
    @IBOutlet weak var timerLabel: UILabel!
    @IBOutlet weak var nameLabel: UILabel!
    @IBOutlet weak var balanceLabel: UILabel!
    
    @IBOutlet weak var containerView: UIView!
    
    // MARK: - Variables
    var selectedItem: ItemModel?
    
    // MARK: Auto Logout
    var autoLogoutTimer: Timer?
    let timeUntilAutoLogoutWarningAppears = 10
    let timeUntilAutoLogoutOccursAfterWarning = 500
    var timeSinceLastUserInteraction = 0 {
        didSet {
            switch timeSinceLastUserInteraction {
            case 0..<timeUntilAutoLogoutWarningAppears:
                timerLabel.text = "Welcome"
            case timeUntilAutoLogoutWarningAppears...timeUntilAutoLogoutWarningAppears + timeUntilAutoLogoutOccursAfterWarning:
                timerLabel.text = "Thank you, logging out in \(timeUntilAutoLogoutWarningAppears + timeUntilAutoLogoutOccursAfterWarning - timeSinceLastUserInteraction) sec."
            default:
                logout()
            }
        }
    }
    
    // MARK: Children View Controllers
    var itemsListViewController = UIStoryboard(name: "Main", bundle: nil).instantiateViewController(withIdentifier: "ItemsListViewController")
    var itemDetailViewController = UIStoryboard(name: "Main", bundle: nil).instantiateViewController(withIdentifier: "ItemDetailViewController")
    
    var currentlyDisplayedViewController: UIViewController?
    
    // MARK: - UIViewController
    override func viewDidLoad() {
        super.viewDidLoad()
        
        (view as? UIUserInteractionCallBackView)?.callback = { [weak self] in
            self?.resetTimer()
        }
        
        autoLogoutTimer = Timer.scheduledTimer(timeInterval: 1, target: self, selector: #selector(timerDidTick(_:)), userInfo: nil, repeats: true)
        
        addChildViewController(itemsListViewController)
        itemsListViewController.willMove(toParentViewController: self)
        containerView.addSubview(itemsListViewController.view)
        constrain(contentView: itemsListViewController.view)
        itemsListViewController.didMove(toParentViewController: self)
        currentlyDisplayedViewController = itemsListViewController
    }
    
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        
        updateHeader()
        
        timeSinceLastUserInteraction = 0
        autoLogoutTimer?.fire()
    }
    
    override func viewDidAppear(_ animated: Bool) {
        super.viewDidAppear(animated)
        
        if UserModel.shared == nil {
            let alert = UIAlertController(title: "Internal Error", message: "Unable to load user", preferredStyle: .alert)
            let okAction = UIAlertAction(title: "Ok", style: .default) { (_) in
                self.logout()
            }
            alert.addAction(okAction)
            present(alert, animated: true, completion: nil)
        }
    }

    override func viewWillDisappear(_ animated: Bool) {
        autoLogoutTimer?.invalidate()
        super.viewWillDisappear(animated)
    }
    
    // MARK: - UI
    func updateHeader() {
        nameLabel.text = UserModel.shared?.netID
        
        if let selectedItem = selectedItem {
            balanceLabel.text = "ℂ\(UserModel.shared?.balance ?? 0) - \(selectedItem.price)"
        } else {
            balanceLabel.text = "ℂ\(UserModel.shared?.balance ?? 0)"
        }
    }
    
    // MARK: - Auto Logout
    func resetTimer() {
        timeSinceLastUserInteraction = 0
    }
    
    func timerDidTick(_ sender: Any) {
        timeSinceLastUserInteraction += 1
    }
    
    // MARK: - Actions
    @IBAction func logout() {
        transition(from: itemDetailViewController, to: itemsListViewController)
        dismiss(animated: true, completion: nil)
    }
    
    // MARK: - Container View Controller
    func transition(from fromViewController: UIViewController, to toViewController: UIViewController) {
        guard fromViewController != toViewController, currentlyDisplayedViewController == fromViewController else { return }
        
        addChildViewController(toViewController)
        
        fromViewController.willMove(toParentViewController: nil)
        toViewController.willMove(toParentViewController: self)
        
        containerView.addSubview(toViewController.view)
        constrain(contentView: toViewController.view)
        containerView.layoutIfNeeded()

        fromViewController.view.alpha = 1
        toViewController.view.alpha = 0
        
        UIView.animate(withDuration: 0.25, animations: { 
            fromViewController.view.alpha = 0
            toViewController.view.alpha = 1
        }) { (_) in
            fromViewController.view.removeFromSuperview()
            fromViewController.removeFromParentViewController()
            toViewController.didMove(toParentViewController: self)
            
            self.constrain(contentView: toViewController.view)
            self.currentlyDisplayedViewController = toViewController
        }
    }
    
    // MARK: - Child View Controller Actions
    func constrain(contentView: UIView) {
        contentView.translatesAutoresizingMaskIntoConstraints = false
        
        containerView.topAnchor.constraint(equalTo: contentView.topAnchor).isActive             = true
        containerView.bottomAnchor.constraint(equalTo: contentView.bottomAnchor).isActive       = true
        containerView.leadingAnchor.constraint(equalTo: contentView.leadingAnchor).isActive     = true
        containerView.trailingAnchor.constraint(equalTo: contentView.trailingAnchor).isActive   = true
    }
    
    func didSelect(item: ItemModel) {
        selectedItem = item
        (itemDetailViewController as? ItemDetailViewController)?.item = item
        transition(from: itemsListViewController, to: itemDetailViewController)
        updateHeader()
    }
    
    func purchasedItem() {
        returnToList()
    }
    
    func purchaseItemFailed(withError error: String) {
        let alert = UIAlertController(title: "Unable to Purchase", message: error, preferredStyle: .alert)
        let okAction = UIAlertAction(title: "Ok", style: .default, handler: nil)
        alert.addAction(okAction)
        present(alert, animated: true, completion: nil)

        returnToList()
    }
    
    func cancelledPurchase() {
        returnToList()
    }
    
    private func returnToList() {
        selectedItem = nil
        transition(from: itemDetailViewController, to: itemsListViewController)
        updateHeader()
    }

}
