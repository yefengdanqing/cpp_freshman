SELECT
    model_name,
    campaign_id,
    COUNT(DISTINCT creative_id) AS creative_id_count,
    SUM(bid) AS total_bid,
    SUM(win) AS total_win,
    SUM(imp) AS total_impressions,
    SUM(click) AS total_clicks,
    SUM(`install`) AS total_installs,

    -- 总 spend 和 revenue 定点小数输出
    CAST(ROUND(SUM(spend)/1000000, 6) AS DECIMAL(20,6)) AS total_spend,
    CAST(ROUND(SUM(revenue), 6) AS DECIMAL(20,6)) AS total_revenue,
    CAST(ROUND(SUM(imp_revenue), 6) AS DECIMAL(20,6)) AS total_imp_revenue,

    SUM(conversion) AS total_conversions,
    SUM(install_by_click) AS total_installs_by_click,
    SUM(request) AS total_request,

    -- 关键指标
    SUM(win) * 100.0 / NULLIF(SUM(bid), 0) AS win_rate,
    SUM(click) * 100.0 / NULLIF(SUM(imp), 0) AS ctr,
    SUM(`install`) * 100.0 / NULLIF(SUM(click), 0) AS cvr,

    CAST(ROUND(SUM(spend) / NULLIF(SUM(click), 0), 6) AS DECIMAL(20,6)) AS cpc,
    CAST(ROUND(SUM(spend) / NULLIF(SUM(`install`), 0), 6) AS DECIMAL(20,6)) AS cpi,
    CAST(ROUND(SUM(revenue) / NULLIF(SUM(spend)/1000000, 0), 6) AS DECIMAL(20,6)) AS roas

FROM dsp.dsp_report
WHERE 
    date = '2025-08-10' 
    AND dc_name IN ('aliyun-sg-k8s') 
    AND ad_exchange = '20020' 
    AND model_name IN ('ST', 'UN')
GROUP BY model_name, campaign_id
HAVING SUM(spend) > 1
ORDER BY total_spend DESC;
